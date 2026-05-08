import cv2
import mediapipe as mp
import math
import time
import serial

# --- CONFIGURACIÓN DEL ARDUINO ---
try:
    arduino = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    time.sleep(2) 
    arduino_conectado = True
    print("Arduino conectado exitosamente.")
except:
    arduino_conectado = False
    print("ADVERTENCIA: Arduino no encontrado. Ejecutando en modo de prueba.")

# --- INICIALIZACIÓN DE MEDIAPIPE ---
mp_face_mesh = mp.solutions.face_mesh
face_mesh = mp_face_mesh.FaceMesh(
    max_num_faces=1,
    refine_landmarks=True,
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5
)

# --- UMBRALES Y TIEMPOS ---
EAR_UMBRAL = 0.25      
MAR_UMBRAL = 0.6       
TIEMPO_ALERTA = 1.5    # Pitido preventivo a los 1.5 segundos
TIEMPO_DETENCION = 3.0 # Detención total a los 3.0 segundos

# Variables de control de estado
inicio_ojos_cerrados = 0
bostezo_en_progreso = False 
vehiculo_detenido = False 

def calcular_distancia(p1, p2):
    return math.dist([p1.x, p1.y], [p2.x, p2.y])

def calcular_ear(ojo, landmarks):
    v1 = calcular_distancia(landmarks[ojo[1]], landmarks[ojo[5]])
    v2 = calcular_distancia(landmarks[ojo[2]], landmarks[ojo[4]])
    h = calcular_distancia(landmarks[ojo[0]], landmarks[ojo[3]])
    return (v1 + v2) / (2.0 * h) if h != 0 else 0

def calcular_mar(boca, landmarks):
    v = calcular_distancia(landmarks[boca[2]], landmarks[boca[3]]) 
    h = calcular_distancia(landmarks[boca[0]], landmarks[boca[1]]) 
    return v / h if h != 0 else 0

# Índices MediaPipe
OJO_IZQ = [33, 160, 158, 133, 153, 144]
OJO_DER = [362, 385, 387, 263, 373, 380]
BOCA = [61, 291, 13, 14] 

cap = cv2.VideoCapture(0, cv2.CAP_V4L2) 
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

print("Sistema iniciado. Presiona 'R' para reiniciar alerta o 'Q' para salir.")

while cap.isOpened():
    ret, frame = cap.read()
    if not ret: break

    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    resultados = face_mesh.process(rgb_frame)

    if resultados.multi_face_landmarks:
        for face_landmarks in resultados.multi_face_landmarks:
            landmarks = face_landmarks.landmark
            ear_promedio = (calcular_ear(OJO_IZQ, landmarks) + calcular_ear(OJO_DER, landmarks)) / 2.0
            mar = calcular_mar(BOCA, landmarks)

            # 1. Lógica de Bostezo (Se envía 'A' una sola vez por bostezo)
            if mar > MAR_UMBRAL:
                cv2.putText(frame, "BOSTEZO DETECTADO", (50, 100), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 165, 255), 2)
                if not bostezo_en_progreso:
                    if arduino_conectado:
                        try: arduino.write(b'A')
                        except: pass
                    bostezo_en_progreso = True 
            else:
                bostezo_en_progreso = False 

            # 2. Lógica de Ojos Cerrados (Microsueños)
            if ear_promedio < EAR_UMBRAL:
                if inicio_ojos_cerrados == 0:
                    inicio_ojos_cerrados = time.time()
                
                tiempo_cerrados = time.time() - inicio_ojos_cerrados
                
                # Detención a los 3 segundos 
                if tiempo_cerrados >= TIEMPO_DETENCION:
                    cv2.putText(frame, "DETENIENDO VEHICULO", (50, 150), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 3)
                    if not vehiculo_detenido:
                        if arduino_conectado:
                            try: 
                                arduino.write(b'S')
                                vehiculo_detenido = True
                            except: pass
                # Alerta a los 1.5 segundos
                elif tiempo_cerrados >= TIEMPO_ALERTA:
                    cv2.putText(frame, "ALERTA: DESPIERTA", (50, 150), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 3)
                    if arduino_conectado:
                        try: arduino.write(b'A')
                        except: pass
            else:
                inicio_ojos_cerrados = 0
                vehiculo_detenido = False 

            cv2.putText(frame, f"EAR: {ear_promedio:.2f}", (30, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
            cv2.putText(frame, f"MAR: {mar:.2f}", (30, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)

    # Lógica de teclado (Reinicio con 'R')
    key = cv2.waitKey(1) & 0xFF
    if key == ord('r') or key == ord('R'):
        print("Reiniciando sistema...")
        inicio_ojos_cerrados = 0
        vehiculo_detenido = False
        bostezo_en_progreso = False
        if arduino_conectado:
            try:
                arduino.reset_input_buffer()
                arduino.write(b'R')
                arduino.flush()
            except: pass
    elif key == ord('q'):
        break

    cv2.imshow("Sistema de Deteccion de Fatiga", frame)

cap.release()
cv2.destroyAllWindows()
if arduino_conectado:
    arduino.close()
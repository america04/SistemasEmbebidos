import cv2
import mediapipe as mp
import math
import time
import serial

try:
    arduino = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    time.sleep(2) # Tiempo para que la conexión serial se estabilice
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
EAR_UMBRAL = 0.25      # Si el EAR es menor a esto, el ojo está cerrado
MAR_UMBRAL = 0.6       # Si el MAR es mayor a esto, está bostezando
TIEMPO_ALERTA = 2.0    # Segundos de ojos cerrados para emitir sonido
TIEMPO_DETENCION = 4.0 # Segundos de ojos cerrados para detener el vehículo

# Variables de control de tiempo
inicio_ojos_cerrados = 0
estado_alerta = False

# --- FUNCIONES MATEMÁTICAS ---
def calcular_distancia(p1, p2):
    """Calcula la distancia euclidiana entre dos puntos (x, y)"""
    return math.dist([p1.x, p1.y], [p2.x, p2.y])

def calcular_ear(ojo, landmarks):
    """
    Calcula el Eye Aspect Ratio (EAR).
    Fórmula: (Distancia vertical 1 + Distancia vertical 2) / (2 * Distancia horizontal)
    """
    # Puntos verticales
    v1 = calcular_distancia(landmarks[ojo[1]], landmarks[ojo[5]])
    v2 = calcular_distancia(landmarks[ojo[2]], landmarks[ojo[4]])
    # Punto horizontal
    h = calcular_distancia(landmarks[ojo[0]], landmarks[ojo[3]])
    
    # Evitar división por cero
    if h == 0:
        return 0
    return (v1 + v2) / (2.0 * h)

def calcular_mar(boca, landmarks):
    """Calcula el Mouth Aspect Ratio (MAR) para detectar bostezos."""
    v = calcular_distancia(landmarks[boca[2]], landmarks[boca[3]]) # Vertical interior
    h = calcular_distancia(landmarks[boca[0]], landmarks[boca[1]]) # Horizontal exterior
    
    if h == 0:
        return 0
    return v / h

# --- ÍNDICES DE PUNTOS FACIALES ---
OJO_IZQ = [33, 160, 158, 133, 153, 144]
OJO_DER = [362, 385, 387, 263, 373, 380]
BOCA = [61, 291, 13, 14] # Izquierda, Derecha, Arriba interior, Abajo interior

# --- BUCLE PRINCIPAL DE VIDEO ---
cap = cv2.VideoCapture(0, cv2.CAP_V4L2) # 0 es la cámara por defecto

# Configuraciones para mejorar la fluidez en la Raspberry Pi 5
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*'MJPG'))

if not cap.isOpened():
    print("Error: No se pudo abrir la webcam USB.")
    
while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        print("No se pudo acceder a la cámara.")
        break

    # Optimización: Redimensionar frame si la Raspberry Pi va lenta
    # frame = cv2.resize(frame, (640, 480))
    
    # Convertir a RGB (MediaPipe lo requiere)
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    resultados = face_mesh.process(rgb_frame)

    if resultados.multi_face_landmarks:
        for face_landmarks in resultados.multi_face_landmarks:
            landmarks = face_landmarks.landmark

            # Calcular métricas
            ear_izq = calcular_ear(OJO_IZQ, landmarks)
            ear_der = calcular_ear(OJO_DER, landmarks)
            ear_promedio = (ear_izq + ear_der) / 2.0
            
            mar = calcular_mar(BOCA, landmarks)

            # --- LÓGICA DE DETECCIÓN DE FATIGA ---
            
            # 1. Detección de Bostezo
            if mar > MAR_UMBRAL:
                cv2.putText(frame, "BOSTEZO DETECTADO", (50, 100), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 165, 255), 2)
                if arduino_conectado:
                    arduino.write(b'A') # Alerta preventiva por bostezo

            # 2. Detección de Ojos Cerrados (Microsueños)
            if ear_promedio < EAR_UMBRAL:
                if inicio_ojos_cerrados == 0:
                    inicio_ojos_cerrados = time.time()
                
                tiempo_cerrados = time.time() - inicio_ojos_cerrados
                
                # Evaluar tiempo con ojos cerrados
                if tiempo_cerrados >= TIEMPO_DETENCION:
                    cv2.putText(frame, "¡PELIGRO! DETENIENDO VEHICULO", (50, 150), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 3)
                    if arduino_conectado:
                        arduino.write(b'S') # Enviar señal de STOP al Arduino
                elif tiempo_cerrados >= TIEMPO_ALERTA:
                    cv2.putText(frame, "¡ALERTA! DESPIERTA", (50, 150), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 3)
                    if arduino_conectado:
                        arduino.write(b'A') # Enviar señal de Alarma al Arduino
            else:
                # Si abre los ojos, reiniciar el contador
                inicio_ojos_cerrados = 0

            # Mostrar métricas en pantalla para depuración
            cv2.putText(frame, f"EAR: {ear_promedio:.2f}", (30, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
            cv2.putText(frame, f"MAR: {mar:.2f}", (30, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)

    # Mostrar la imagen
    cv2.imshow("Sistema de Deteccion de Fatiga", frame)

    # Presionar 'q' para salir
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Limpieza
cap.release()
cv2.destroyAllWindows()
if arduino_conectado:
    arduino.close()
void task()
{
    // Definición de un enumerado para representar los estados de la tarea
    enum class TaskStates
    {
        INIT,       // Estado de inicialización
        WAIT_INIT,  // Estado de espera de inicialización
        SEND_EVENT  // Estado de envío de evento
    };

    // Variables estáticas para mantener el estado y otros datos entre llamadas a la función
    static TaskStates taskState = TaskStates::INIT;  // Estado inicial: INIT
    static uint32_t previous = 0;  // Variable para almacenar el tiempo anterior
    static u_int32_t counter = 0;   // Contador de eventos

    // Switch-case para manejar el comportamiento de la tarea según su estado
    switch (taskState)
    {
        case TaskStates::INIT:
        {
            // Inicialización de la comunicación serial
            Serial.begin(115200);
            // Cambio de estado a WAIT_INIT
            taskState = TaskStates::WAIT_INIT;
            break;
        }
        case TaskStates::WAIT_INIT:
        {
            // Espera de una señal desde el monitor serial
            if (Serial.available() > 0)
            {
                // Si se recibe '1', se cambia al estado SEND_EVENT
                if (Serial.read() == '1')
                {
                    previous = 0;  // Forzar el envío del primer evento inmediatamente
                    taskState = TaskStates::SEND_EVENT;
                }
            }
            break;
        }
        case TaskStates::SEND_EVENT:
        {
            // Obtener el tiempo actual en milisegundos
            uint32_t current = millis();
            // Si ha pasado más de 2000 ms desde el evento anterior, se envía un nuevo evento
            if ((current - previous) > 2000)
            {
                previous = current;  // Actualizar el tiempo anterior
                Serial.print(counter);  // Envío del evento
                counter++;  // Incremento del contador
            }
            // Si se recibe '2' desde el monitor serial, se vuelve al estado WAIT_INIT
            if (Serial.available() > 0)
            {
                if (Serial.read() == '2')
                {
                    taskState = TaskStates::WAIT_INIT;
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

void setup()
{
    // Llamada a la función task() en el momento de la inicialización del programa
    task();
}

void loop()
{
    // Llamada continua a la función task() en el bucle principal
    task();
}
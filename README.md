# Water Waker (Introduction)

Water Waker is an alarm clock supplemented with a water spraying function to help wake up users. Taking inspiration from the refreshing sensation of splashing water on one's face in the morning, the Water Waker will spray the user's face with a brisk burst of water when the alarm goes off.

# Hardware Design and Implementation Details

The product consists of ESP32 as computing device, a water pump attached to servo as electro-mechanical properties, mini water tank, LCD 1602a as a time indicator and buzzer/beeper as audio device. 

![Schematic](images/Hardware-Prototype.png)

As visualized in the schematic/wiring diagram above, our prototype uses four signal pins, and two power pins. Pin D2 is connected to a buzzer to trigger it when the event occurs. Pin D21 and D22, works as medium to transmit SDA (Serial Data) and SCL (Serial Clock), respectively, in an I2C communication between ESP32 and 1602a LCD. Last, Pin D13 works to transmit PWM signal to the servo, so that servo can pull the spray trigger.

# Network Infrastructure

As for network infrastructure, our product has ESP32, Blynk platform and user's smartphone. Both ESP32 and smartphone acts as a client, means that those device are either actively sending request or receiving data from another client. On the other hand, Blynk platform acts as a server. It manages the communication between the ESP32 and the smartphone app, processes incoming data, and sends commands or updates to the ESP32.

# Software Implementation details
The software development phase of the Water Waker project focused on crafting a robust program responsible for orchestrating and managing the intricate hardware components essential for its functionality. Implemented in Arduino, leveraging the familiarity and versatility of the C/C++ programming language, the software operates as the brain of the system, coordinating the microcontroller, beeper, and water pump seamlessly. The program's design centers on direct interaction with hardware elements, dictating the precise moments for the beeper to emit sound and orchestrating the water pump's activation to dispense water. The software execution follows a meticulously planned flowchart. It begins with initializing the LCD to display a reassuring 'Alarm Water Waker ready' message, proceeding to establish a Wi-Fi connection. Upon successful connection verification, the LCD confirms 'Wifi Connected', initiating Blynk services. Subsequently, the software configures the system's time, retrieves the current time, and presents it on the LCD. Simultaneously, Blynk integration facilitates the display of the set alarm time and allows users to configure the alarm settings. Ultimately, this intricately woven software orchestrates the Water Waker's operations, seamlessly executing the alarm functionalities while ensuring user interactivity through the Blynk platform

# Test Result and Performance Evaluation
## Testing Summary
We thoroughly tested the Water Waker to ensure it met our expectations. Tests included checking the alarm's volume, the reach of the water spray, and the ease of setting the alarm.

### Key Findings
- **Alarm Loudness:** Verified it could wake users effectively.
- **Water Spray Range:** Ensured the water reached its intended target.
- **Simple Alarm Setting:** Tested for user-friendly alarm configuration.

### Results
The Water Waker performed admirably, effectively waking users with both sound and water. It was straightforward to use and didn't incur high costs.

- **Effective Functionality:** Successful wake-up using sound and water.
- **User-Friendly:** Easy-to-use controls for alarm settings.
- **Cost-Effective:** Developed affordably without compromising functionality.

### Testing Outcome
All tests confirmed the Water Waker's performance as intended, particularly highlighting the unique water-spraying feature.

# Conclusion and future work

The “Water Waker” project was created to help solve the issue of sleeping too much, a problem that’s common among college students. The goal was to make an alarm that wakes people up by spraying water. The project showed how IoT can be used in everyday life, changing the way we wake up in the morning.

However, there is, still, a room for improvement. Several potential improvements and future work can be considered for the Water Waker project, such as increasing water capacity, improving battery life and customizable alarm settings.

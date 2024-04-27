# MCP2518FD-on-Raspberry-Pico
Simple implementation of CAN BUS logger with Raspberry Pico and MCP2518FD.
Introducing the Raspberry Pico CAN FD Logger – a compact yet powerful tool for logging Controller Area Network (CAN) data with Flexible Data Rate (FD) support. This logger, built using the Raspberry Pico microcontroller, allows you to capture and store CAN bus messages efficiently.

Equipped with a CAN transceiver, the Raspberry Pico CAN FD Logger seamlessly interfaces with your vehicle's CAN network, enabling you to monitor and record communication between various electronic control units (ECUs). With its small form factor and low power consumption, it's perfect for automotive diagnostics, vehicle performance analysis, and more.

Using C/C++ programming languages, you can customize the logger to suit your specific needs. Whether you're a hobbyist, automotive enthusiast, or professional engineer, the Raspberry Pico CAN FD Logger offers an affordable and versatile solution for CAN bus logging.

The Raspberry Pico CAN FD Logger was developed using Visual Studio Code alongside the PICO SDK, providing a streamlined and efficient development environment. Leveraging the powerful features of Visual Studio Code, such as IntelliSense and debugging capabilities, developers could easily write, test, and debug their code for the Raspberry Pico microcontroller.

With the PICO SDK integration, developers had access to a rich set of libraries and APIs tailored specifically for the Raspberry Pico platform. This allowed for seamless interaction with the microcontroller's peripherals, including GPIOs, UART, SPI, and, crucially, the CAN controller.

By combining the versatility of Visual Studio Code with the flexibility of the PICO SDK, the development team was able to rapidly prototype and iterate on the CAN FD Logger firmware. The result is a robust and feature-rich logging solution that meets the needs of both hobbyists and professionals in the automotive industry.

# Hardware implementation

Raspberry Pi Pico W: As the central microcontroller, it runs your application code, processes data, and manages Wi-Fi connectivity. It's ideal for tasks requiring wireless communication or remote data monitoring.

[Dual CAN PICO V1.0](https://www.sg-electronic-systems.com/dual-can-pico-v1-0/) : This carrier board acts as the bridge between your Raspberry Pi Pico W and the CAN BUS modules. It simplifies the hardware connections and potentially the software integration as well, making it easier to connect everything together and maintain a compact and efficient design.


[CAN BUS FD V1.4](https://www.sg-electronic-systems.com/can-bus-fd-v1-4-shield-for-raspberry-pi/) : This module is specifically designed for high-performance CAN communications, supporting the CAN FD protocol. It allows for faster data transmission rates compared to classic CAN, which is beneficial in environments where devices need to transmit a high volume of data rapidly or where greater bandwidth is required.

This setup allows the Raspberry Pi Pico W to interact with CAN networks via the Dual CAN PICO V1.0 and CAN BUS FD V1.4. This can be particularly useful in automotive applications, smart city infrastructure, or any industrial system where devices are spread out over large areas and need to communicate reliably and efficiently.

![Picture of HW setup](https://www.sg-electronic-systems.com/wp-content/uploads/2024/04/Dual-Can-FD-on-Pico.jpg)

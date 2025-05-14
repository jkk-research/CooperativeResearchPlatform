# Cooperative Research Platform
[![Build test](git@github.com:jkk-research/CooperativeResearchPlatform/actions/workflows/build_test.yml/badge.svg)](git@github.com:jkk-research/CooperativeResearchPlatform/actions/workflows/build_test.yml)
## Overview
This platform is developed and maintained by University of Győr - Vehicle Research Center in cooperation with ZalaZONE Innovation Park. The project is supported by Robert Bosch Engineering Center Budapest.\
<br />
![alt text](images/crp_logos.png "Partners")
<br />
\
**Our mission: to provide end-to-end vehicle control solutions to cover multiple driving scenarios, special focus on use-cases defined by industrial needs, utilizing cutting edge solution in terms communication, sensing, planning and control.**\
\
The platform is designed to be independent from any vehicle platform but providing a so called vehicle integration layer to adapt external signals to the controller based software components.\
We use ROS2 as middle-ware.

![alt text](images/abstraction_layers.png "Abstraction layers of CRP")

```mermaid
---
title: Vehicle_Integration_Layer
---
graph LR
        CAN_Driver_Left[CAN<br>driver]:::light
        Sensor_Driver1[Sensor<br>driver 1]:::light
        Sensor_Driver2[Sensor<br>driver 2]:::light
        CAN_Driver_Right[CAN<br>driver]:::light
        CAN_Driver_Left ---> Sensor_Abstraction
        Sensor_Driver1 ---> Sensor_Abstraction
        Sensor_Driver2 ---> Sensor_Abstraction
        subgraph Controller_Integration_Layer
            direction TB
            Configuration[Configuration]:::dark
            Sensor_Abstraction[Sensor<br>abstraction]:::dark
            Actuator_Abstraction[Actuator<br>abstraction]:::dark
            Sensor_Abstraction --> Topic_Handler1
            Configuration -.-> Sensor_Abstraction
            Topic_Handler2 --> Actuator_Abstraction
            subgraph Application_Layer
                Topic_Handler1[Topic<br>handler]:::white
                Function_Code[Function<br>Code]:::white
                Topic_Handler2[Topic<br>handler]:::white
                Topic_Handler1 --> Function_Code
                Function_Code --> Topic_Handler2
                end
            end
        Actuator_Abstraction --> CAN_Driver_Right


classDef light fill:#34aec5,stroke:#152742,stroke-width:2px,color:#152742  
classDef dark fill:#152742,stroke:#34aec5,stroke-width:2px,color:#34aec5
classDef white fill:#ffffff,stroke:#152742,stroke-width:2px,color:#152742
classDef red fill:#ef4638,stroke:#152742,stroke-width:2px,color:#fff
```

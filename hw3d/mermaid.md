```mermaid
classDiagram
    class Vehicle {
        +int speed
        +run()
    }

    class Car {
        +String model
        +drive()
    }

    Vehicle <|-- Car
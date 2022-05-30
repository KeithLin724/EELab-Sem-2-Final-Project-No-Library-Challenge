# EELab Sem 2 final project 2022 No library challenge 

#### Written by **KYLiN** 
#### gifted to ***MP Liew*** 

---

You only need to download the library call ```TimerOne```

This library is using for display function in cycle 

You can clack this link to know more about how to download library 

- TimerOne
- - https://github.com/PaulStoffregen/TimerOne
  
- Download doc
- - https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries

---
# Setting pin 
About the setting pin 

> **Display pin**
![](/readme_img/display_change_pin.png)

> **Button pin setting**
![](/readme_img/button_setting.png)

> **Temp setting pin**
![](/readme_img/temp_setting.png)

> **Distance Setting**
![](/readme_img/TriEcho.png)

--- 
# Flow chart 
```mermaid 
flowchart 
    ButtonMode
    
    subgraph var
    Distance
    temp
    end

    subgraph Timer
    TimerOne
    time_75ms
    end
    Main-->Display
    Timer-->Display_Arrays
    control_block

    ButtonMode-->control_block
    Distance -->control_block
    temp-->control_block

    
    control_block-->choice-->Display_Arrays
    Display_Arrays-->Display
```



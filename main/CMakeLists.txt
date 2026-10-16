idf_component_register(
    SRCS 
    "main.cpp"
    "sensors/barometer.cpp"                   
    PRIV_REQUIRES spi_flash esp_driver_gpio
                       
    INCLUDE_DIRS "")

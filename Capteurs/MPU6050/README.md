## Le capteur MPU5060

    i2cdetect -y 1

 - l'adresse I2C du composant est 0x68

Lecture du registre 0x75 qui doit retourner l'adresse


    i2cget -y 1 0x68 0x75


Ecriture de 0x00 dans le registre d'alimentation PWR_MGMT_1

    i2cset -y 1 0x68 0x6B 0x00

Lecture des registres 0x3F 0x40 accélération sur axe Z

    i2cget -y 1 0x68 0x3F w


# Proyecto de Sistemas Operativos
Hola bueno este es el proyecto de Sistemas Operativos, basicamente el proceso fue planear todo primero de como lo ibamos a hacer, para esto primero vimos el proyecto
y como estaba estructurado para la entrega con sus condiciones y nos basamos en eso, despues en las clases se explico un poco mas a fondo ayudas para el proyecto y fueron tomadas en cuenta
empezamos primero entonces por linux el cual ya teniamos avanzado algo de ahce 3 semanas aproximadamente, se termina, y despues se pasa a windows

## LINUX
Aqui estan los pasos a seguir para poder correr los archivos de manera correcta

### 1. Instalar WSL

Ejecuta en PowerShell como Administrador:

wsl --install


Esto instalará automáticamente:

WSL2 y Ubuntu

Reinicia.

### 2. Abrir Ubuntu por primera vez

Busca “Ubuntu” en el menú iniciar y pues obvcio registrarse por primera vez

### 3. Instalar herramientas esenciales dentro de Ubuntu

Ejecutar:

sudo apt update
sudo apt install build-essential gdb


Esto instala:

gcc

g++

make

gdb

### 4. Instalar y configurar VS Code para trabajar con WSL

En Windows:

Abre VS Code.

Instala la extensión oficial: "WSL"


Ctrl + Shift + P


Escribir:

Remote-WSL: New WSL Window


Ahora VS Code está conectado directamente al Linux real del WSL.

### 5. Crear tu carpeta de trabajo en Ubuntu

En Ubuntu:

mkdir -p ~/Practicas_1_S.O.
cd ~/Practicas_1_S.O.


En VS Code (ya conectado a WSL):

File → Open Folder → selecciona /home/dacruzj/Practicas_1_S.O.

### 6. Crear estructura correcta del proyecto

Dentro de VS Code:

<img width="307" height="641" alt="image" src="https://github.com/user-attachments/assets/8dcd9d19-dc70-41ab-88dd-017ed5be0856" />



### 7. Crear el Makefile
<img width="458" height="444" alt="image" src="https://github.com/user-attachments/assets/e259d6d8-4337-4668-953e-d4f404a25d08" />

### 8. Compilar todo

En Ubuntu:

make clean
make


Si todo está bien, aparecerán:

lib/libproceso_par.a
test/test_comm
test/hijo


### 9. Ejecutar el programa padre (test_comm)

Aquí se prueba todo el sistema:

./test/test_comm


La salida correcta debería ser algo como:

<img width="681" height="204" alt="image" src="https://github.com/user-attachments/assets/85c03dbd-c23e-4d97-86b4-431c4a334904" />

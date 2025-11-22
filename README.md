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

## Windows

Pasos a seguir para la ejecucion correcta

### Primera alternativa

###1.	Abre la terminal MSYS2 MinGW x64

Ve a la carpeta donde se clonó el proyecto. Asegúrate de tener gcc, ar y make instalados.
En la primera alternativa usando el Makefile, entra en la carpeta Windows/src y ejecuta 
mingw32-make clean y luego mingw32-make, lo cual generará la librería en la carpeta lib.

###2. Luego compila el demo 

Con el comando: gcc -Wall -Wextra -O2 -I../include main.c -L../lib -lproceso_par -o demo.exe -lws2_32.

### 3. Después compila el programa de eco 

Usando: gcc -Wall -Wextra -O2 eco_win.c -o eco.exe -lws2_32.

### 4.	Para probar

Ejecuta ./demo.exe o ./eco.exe según lo que quieras validar.

 ##Segunda alternativa
 ## 1. Abre nuevamente la terminal MSYS2 MinGW x64
 
 Ve a la carpeta del proyecto. Como el código usa la cabecera <winsock2.h>, debes enlazar con la librería ws2_32.
 en la carpeta Windows/src compila manualmente la librería con: gcc -c -Wall -Wextra -O2 -I../include proceso_par_win.c -o proceso_par_win.o 
 y crea la librería con: ar rcs ../lib/libproceso_par.a proceso_par_win.o, lo cual generará el archivo libproceso_par.a en la carpeta lib.
 
 ## 2.Luego compila el demo
 
 Usando nuevamente: gcc -Wall -Wextra -O2 -I../include main.c -L../lib -lproceso_par -o demo.exe -lws2_32.
 
## 3. Crear un archivo de prueba 

Usar el comando cat desde la terminal, y luego compilar el programa de eco con: gcc -Wall -Wextra -O2 eco_win.c -o eco.exe -lws2_32.

## 4. Finalmente 

Prueba todo ejecutando ./demo.exe.



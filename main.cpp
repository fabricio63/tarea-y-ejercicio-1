#include <iostream>
#include <vector>
#include <time.h>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <signal.h>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
typedef int (*proceso)();
int print_kernel();
uint32_t tiempo_quantum();
proceso context_switch();
#define cantidad_procesos 5
int contador_U[cantidad_procesos];
#define indice_contextswitching 0
#define indice_printer 1
int contador_kernel[2];
vector<proceso> listos;
int ProcesoUsuario_1();
int ProcesoUsuario_2();
int ProcesoUsuario_3();
int ProcesoUsuario_4();
int ProcesoUsuario_5();
int inicio_kernel();




int ProcesoUsuario_1() {
#define indice_proceso1 0
    cout << "ProcesoUsuario_1()" << endl;
    contador_U[indice_proceso1]++;
    return 0;
}
int ProcesoUsuario_2() {
#define indice_proceso2 1
    cout << "ProcesoUsuario_2()" << endl;
    contador_U[indice_proceso2]++;
    return 0;
}
int ProcesoUsuario_3() {
#define indice_proceso3 2
    cout << "ProcesoUsuario_3()" << endl;
    contador_U[indice_proceso3]++;
    return 0;
}
int ProcesoUsuario_4() {
#define indice_proceso4 3
    cout << "ProcesoUsuario_4()" << endl;
    contador_U[indice_proceso4]++;
    return 0;
}
int ProcesoUsuario_5() {
#define indice_proceso5 4
    cout << "ProcesoUsuario_5()" << endl;
    contador_U[indice_proceso5]++;
    return 0;
}
int print_kernel() {
    for (uint32_t i {0}; i < cantidad_procesos; i++ ) {
        cout << " valor:" << contador_U[i] << endl;
    }
    contador_kernel[indice_printer]++;
    return 0;
}
vector<proceso> vector_procesos_usuario = {ProcesoUsuario_1, ProcesoUsuario_2, ProcesoUsuario_3, ProcesoUsuario_4, ProcesoUsuario_5, print_kernel};

int inicio_kernel() {
    cout << "kernel..." << endl;
    return 0;
}

int admision_procesos();
int admision_procesos() {
    proceso u_process { vector_procesos_usuario.at(0) };
    vector_procesos_usuario.erase(vector_procesos_usuario.begin());
    listos.push_back(u_process);
    return 0;
}


proceso context_switch() {
    cout << "context switch()" << endl;
    if (!listos.empty()) {
        uint32_t index { 0 };
        proceso removidos { listos.at(index) };
        listos.erase(listos.begin());
        listos.push_back(removidos);
    }
    contador_kernel[indice_contextswitching]++;
    proceso test;
    return test;
}

uint32_t tiempo_quantum() {

    srand(time(NULL));
    return ((rand() % 9) + 1);
}

void print_csv(int sig){
    std::ofstream file;
    file.open("data.csv");
    file << "proceso;tipo;id;quantum;valor;conmutacion\n";
    file << "u_process1;user;"      << &ProcesoUsuario_1  << "1-10;" <<  contador_U[0]  <<  contador_U[0]  << "\n";
    file << "u_process2;user;"      << &ProcesoUsuario_2   << "1-10;" <<  contador_U[1] <<  contador_U[1] << "\n";
    file << "u_process3;user;"      << &ProcesoUsuario_3    << "1-10;" <<  contador_U[2] <<  contador_U[2]  << "\n";
    file << "u_process4;user;"      << &ProcesoUsuario_4 << "1-10;" <<  contador_U[3] <<  contador_U[3] << "\n";
    file << "u_process5;user;"      << &ProcesoUsuario_5 << "1-10;" <<  contador_U[4]  << contador_U[4] << "\n";
    file.close();
    exit(1);
}

int main() {

    memset(contador_U, 0, sizeof(contador_U));
    memset(contador_kernel, 0, sizeof(contador_kernel));
    listos.push_back(&inicio_kernel);
    proceso func;
    uint32_t quantum { 0 };
    bool una_iteracion {false};
    signal(SIGINT, print_csv);
    while (2) {
        if (una_iteracion) {
            una_iteracion = false;
            continue;
        }
        if (!vector_procesos_usuario.empty()) {
            func = admision_procesos;
            func();
            continue;
        }
        func = listos.at(0);
        func();
        if (func == &inicio_kernel) {
            listos.erase(listos.begin());
            quantum = tiempo_quantum();
            continue;
        }
        if (quantum == 0) {
            quantum = tiempo_quantum();
            func = context_switch();
            una_iteracion = true;
            continue;
        }
        quantum--;
    }
    
    return 0;
}


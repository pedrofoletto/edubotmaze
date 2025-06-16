#include <iostream>
#include "libs/EdubotLib.hpp"

using namespace std;

#define DISTANCIA 0.18

// Função auxiliar para contar quantos sensores estão livres (se os 3 sensores estiverem livres, achamos a saida!)
int contarSaidasLivres(EdubotLib *edubotLib) {
    int saida = 0;
    for (int i = 0; i <= 6; i += 3) {
        if (edubotLib->getSonar(i) >= 2) {
            saida++;
        }
    }
    return saida;
}

// Função que faz o robô avançar até encontrar uma parede à frente
void avancarAteParede(EdubotLib *edubotLib) {
    while (edubotLib->getSonar(3) > DISTANCIA) { 
        edubotLib->move(0.3); 
        edubotLib->sleepMilliseconds(100);
        if(contarSaidasLivres(edubotLib)>=3){ 
        	edubotLib->stop();
        	edubotLib->sleepMilliseconds(1000);
        	break;
        }
        
    }
}


int main() {
    EdubotLib *edubotLib = new EdubotLib(); 
    
    if(edubotLib->connect()) {

        // Anda para frente até encontrar uma parede
        edubotLib->move(0.3); 
        edubotLib->sleepMilliseconds(100);
        avancarAteParede(edubotLib);
        
        while (contarSaidasLivres(edubotLib) < 3) {

            // Leitura dos sensores laterais
            double distParedeDireita = edubotLib->getSonar(6);
            double distParedeEsquerda = edubotLib->getSonar(0);

            // Se houver parede à direita e espaço à esquerda = vira para esquerda
            if (distParedeDireita <= DISTANCIA && distParedeEsquerda >= DISTANCIA) {
                edubotLib->rotate(-90);
                edubotLib->sleepMilliseconds(2000);
                avancarAteParede(edubotLib); 
                
            // Se houver parede à esquerda e espaço à direita = vira para direita
            } else if (distParedeDireita >= DISTANCIA && distParedeEsquerda <= DISTANCIA) {
                edubotLib->rotate(90);
                edubotLib->sleepMilliseconds(2000);
                avancarAteParede(edubotLib); 
                
            // Se ambos os lados estiverem livres = vira para direita
            } else if (distParedeDireita > DISTANCIA && distParedeEsquerda > DISTANCIA){
                edubotLib->rotate(90);
                edubotLib->sleepMilliseconds(2000);
                avancarAteParede(edubotLib); 
            }
        }
        
        std::cout << "Saída encontrada!" << std::endl;
        
        edubotLib->disconnect();
    } else {
        std::cout << "Could not connect on robot!" << std::endl;
    }

    return 0;
}

# Desafio Sillion

# Controlador Temporizado de Motor DC com Proteção de Corrente

Este projeto consiste no desenvolvimento de um firmware para um sistema embarcado dedicado, cujo objetivo é acionar uma carga  (Motor DC 5V/1A) por um período determinado de 10 segundos, monitorando ativamente a corrente para proteção contra falhas.

##  Objetivo do Projeto

Projetar um sistema robusto capaz de:
1.  Ler um sinal de entrada em nível lógico baixo (botão/sinal 12V isolado opticamente).
2.  Acionar um Motor DC (5V, 1A) através de um driver MOSFET.
3.  Monitorar a corrente em tempo real e desligar o sistema  em caso de sobrecorrente (> 1.5A).

##  Software

Embora a lógica de controle siga estritamente a estrutura da linguagem C, foi utilizado o Framework Arduino como camada de abstração de hardware (HAL).

**Justificativa Técnica:**
* **Abstração de Hardware (HAL):** O framework fornece uma interface segura para manipulação de I/O e ADC, garantindo que o acionamento dos periféricos do ATmega328P seja feito de forma confiável sem a necessidade de reescrever drivers de baixo nível (registradores).
* **Eficiência:** Desenvolver em C Puro (Bare Metal) exigiria um tempo significativo para configurar drivers de comunicação (UART) e timers.
* **Gravação:** O projeto foi desenhado para ser gravado externamente, dispensando interfaces USB complexas na PCB final. O código resultante é leve, seguro e reutilizável.




O firmware opera em um loop contínuo (Polling) com verificação de estados. Abaixo, o detalhamento das principais funções:


### 1. Controle de Estado (`ativa_motor` / `desliga_motor`)
Funções responsáveis pelo chaveamento físico do MOSFET e feedback visual (LED).
* Ao ativar: Captura o `millis()` atual como tempo zero (timestamp) e notifica via Serial.
* Ao desligar: Corta a alimentação do gate do MOSFET e reseta a flag de estado.

### 2. Sistema de Proteção (`protc_leitura`)
Esta é a função de segurança. O circuito utiliza um resistor Shunt de 0.1Ω e um Amplificador Operacional com ganho de 21x.

Se a leitura do ADC ultrapassar 645, o sistema interpreta como travamento ou curto-circuito e desliga a carga.


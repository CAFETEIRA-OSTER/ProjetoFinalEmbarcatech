#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "lwip/tcp.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2818b.pio.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"




#define LEDG_PIN 11
#define LEDR_PIN 13
#define BTN_B 6
#define LED_COUNT 25
#define LED_PIN 7
#define DEBOUNCE_DELAY 50
#define WIFI_SSID "GalaxyS20FE"  // Substitua pelo nome da sua rede Wi-Fi
#define WIFI_PASS "244466666" // Substitua pela senha da sua rede Wi-Fi


// Configuração do pino do buzzer
#define BUZZER_PIN 21


const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

// Configuração da frequência do buzzer (em Hz)
int BUZZER_FREQUENCY = 12000;
int tentativas = 0;
int concatenacao = 0;
int digitos[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
int primeiro_digito = 0;
int status = 0;
int dificuldade_por_tempo = 200;

/* Buffer para respostas HTTP
const char *html_content = "<!DOCTYPE html>\n"
                         "<html>\n"
                         "<head>\n"
                         "    <meta charset=\"UTF-8\">\n"
                         "    <title>Projeto final - EMBARCATECH</title>\n"
                         "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                         "    <style>\n"
                         "        body {\n"
                         "            display: flex;\n"
                         "            flex-direction: column;\n"
                         "            justify-content: center;\n"
                         "            align-items: center;\n"
                         "            min-height: 100vh;\n"
                         "            margin: 0;\n"
                         "            background-color: #006494;\n"
                         "            font-family: 'Arial', sans-serif;\n"
                         "        }\n"
                         "\n"
                         "        .rounded-rectangle {\n"
                         "            width: 90%;\n"
                         "            max-width: 70vh;\n"
                         "            background-color: #f0f0f0;\n"
                         "            border-radius: 20px;\n"
                         "            box-shadow: 5px 5px 10px rgba(0, 0, 0, 0.2);\n"
                         "            padding: 20px;\n"
                         "            display: flex;\n"
                         "            flex-direction: column;\n"
                         "            align-items: center;\n"
                         "            margin-bottom: 20px;\n"
                         "        }\n"
                         "\n"
                         "        .rectangle-text {\n"
                         "            font-size: 1.5rem;\n"
                         "            font-weight: bold;\n"
                         "            color: #333;\n"
                         "            text-align: center;\n"
                         "            margin-bottom: 20px;\n"
                         "        }\n"
                         "\n"
                         "        .input-container {\n"
                         "            width: 90%;\n"
                         "            margin-bottom: 10px;\n"
                         "            position: relative;\n"
                         "        }\n"
                         "        .input-container-observacoes {\n"
                         "            width: 90%;\n"
                         "            margin-bottom: 10px;\n"
                         "            position: relative;\n"
                         "        }\n"
                         "\n"
                         "        .styled-input {\n"
                         "            width: 100%;\n"
                         "            padding: 10px;\n"
                         "            border: 1px solid #ccc;\n"
                         "            border-radius: 10px;\n"
                         "            box-sizing: border-box;\n"
                         "            font-size: 1rem;\n"
                         "            transition: border-color 0.3s;\n"
                         "        }\n"
                         "\n"
                         "        .styled-input:focus {\n"
                         "            border-color: #006494;\n"
                         "            outline: none;\n"
                         "        }\n"
                         "\n"
                         "        .styled-input-observacoes {\n"
                         "            width: 100%;\n"
                         "            padding: 10px;\n"
                         "            border: 1px solid #ccc;\n"
                         "            border-radius: 10px;\n"
                         "            box-sizing: border-box;\n"
                         "            font-size: 1rem;\n"
                         "            transition: border-color 0.3s;\n"
                         "            resize: vertical;\n"
                         "        }\n"
                         "\n"
                         "        .styled-input-observacoes:focus {\n"
                         "            border-color: #006494;\n"
                         "            outline: none;\n"
                         "        }\n"
                         "\n"
                         "        .button-container {\n"
                         "            display: flex;\n"
                         "            justify-content: space-around;\n"
                         "            width: 100%;\n"
                         "            align-items: center;\n"
                         "            flex-wrap: wrap;\n"
                         "        }\n"
                         "\n"
                         "        .styled-button {\n"
                         "            padding: 10px 20px;\n"
                         "            border: none;\n"
                         "            border-radius: 10px;\n"
                         "            font-size: 1rem;\n"
                         "            cursor: pointer;\n"
                         "            transition: background-color 0.3s;\n"
                         "            margin: 5px;\n"
                         "        }\n"
                         "         .styled-button:active {\n"
                         "            transform: scale(0.95);\n"
                         "        }\n"
                         "\n"
                         "\n"
                         "        .blue-button {\n"
                         "            background-color: #007bff;\n"
                         "            color: white;\n"
                         "        }\n"
                         "\n"
                         "        .gray-button {\n"
                         "            background-color: #888;\n"
                         "            color: white;\n"
                         "        }\n"
                         "\n"
                         "        .gray-bar {\n"
                         "            width: 90%;\n"
                         "            height: 5px;\n"
                         "            background-color: #ccc;\n"
                         "            border-radius: 5px;\n"
                         "            margin-top: 30px;\n"
                         "            position: relative;\n"
                         "            overflow: hidden;\n"
                         "        }\n"
                         "\n"
                         "        .progress-bar {\n"
                         "            width: 0%;\n"
                         "            height: 100%;\n"
                         "            background-color: #007bff;\n"
                         "            border-radius: 5px;\n"
                         "            transition: width 2s ease-in-out;\n"
                         "        }\n"
                         "        @media (max-width: 768px) {\n"
                         "             .rectangle-text {\n"
                         "                 font-size: 1.2rem;\n"
                         "            }\n"
                         "            .styled-button{\n"
                         "              font-size: 0.9rem;\n"
                         "            }\n"
                         "           .rounded-rectangle {\n"
                         "             width: 95%;\n"
                         "             padding: 10px;\n"
                         "            }\n"
                         "\n"
                         "            .button-container {\n"
                         "                flex-direction: column;\n"
                         "            }\n"
                         "            .styled-button{\n"
                         "              width: 80%;\n"
                         "            }\n"
                         "\n"
                         "        }\n"
                         "    </style>\n"
                         "</head>\n"
                         "<body>\n"
                         "    <div class=\"rounded-rectangle\">\n"
                         "        <div class=\"rectangle-text\">INTERFACE DE CONFIGURAÇÃO</div>\n"
                         "        <div class=\"input-container\">\n"
                         "            <input type=\"text\" id=\"nomeJogador\" class=\"styled-input\" placeholder=\"Insira o nome do jogador\">\n"
                         "        </div>\n"
                         "        <div class=\"input-container\">\n"
                         "            <input type=\"text\" id=\"velocidadeJogo\" class=\"styled-input\" placeholder=\"Insira a velocidade do jogo (200) = 0.2 segundos\">\n"
                         "        </div>\n"
                         "        <div class=\"button-container\">\n"
                         "            <button class=\"styled-button blue-button\">Enviar Dados</button>\n"
                         "            <button class=\"styled-button gray-button\" onclick=\"gerarPDF()\">Baixar Arquivo</button>\n"
                         "        </div>\n"
                         "        <div class=\"gray-bar\">\n"
                         "            <div class=\"progress-bar\"></div>\n"
                         "        </div>\n"
                         "    </div>\n"
                         "\n"
                         "    <div class=\"rounded-rectangle\">\n"
                         "        <div class=\"rectangle-text\">INFORMAÇÕES</div>\n"
                         "        <div class=\"input-container\">\n"
                         "            <input type=\"text\" id=\"score1\" class=\"styled-input\" placeholder=\"Pontuação 1\">\n"
                         "        </div>\n"
                         "        <div class=\"input-container\">\n"
                         "            <input type=\"text\" id=\"score2\" class=\"styled-input\" placeholder=\"Pontuação 2\">\n"
                         "        </div>\n"
                         "        <div class=\"input-container\">\n"
                         "            <input type=\"text\" id=\"score3\" class=\"styled-input\" placeholder=\"Pontuação 3\">\n"
                         "        </div>\n"
                         "        <div class=\"input-container-observacoes\">\n"
                         "            <textarea id=\"observacoes\" class=\"styled-input-observacoes\" placeholder=\"Observações\"></textarea>\n"
                         "        </div>\n"
                         "    </div>\n"
                         "\n"
                         "    <script>\n"
                         "        function gerarPDF() {\n"
                         "            const { jsPDF } = window.jspdf;\n"
                         "            const doc = new jsPDF();\n"
                         "\n"
                         "            let nomeJogador = document.getElementById(\"nomeJogador\").value || \"Não informado\";\n"
                         "            let velocidadeJogo = document.getElementById(\"velocidadeJogo\").value || \"Não informado\";\n"
                         "            let score1 = document.getElementById(\"score1\").value || \"Não informado\";\n"
                         "            let score2 = document.getElementById(\"score2\").value || \"Não informado\";\n"
                         "            let score3 = document.getElementById(\"score3\").value || \"Não informado\";\n"
                         "            let observacoes = document.getElementById(\"observacoes\").value || \"Não informado\";\n"
                         "\n"
                         "            doc.setFont(\"helvetica\", \"bold\");\n"
                         "            doc.setFontSize(18);\n"
                         "            doc.text(\"Configuração do Jogo\", 20, 20);\n"
                         "            doc.setFontSize(12);\n"
                         "            doc.setFont(\"helvetica\", \"normal\");\n"
                         "            doc.text(`Nome do Jogador: ${nomeJogador}`, 20, 40);\n"
                         "            doc.text(`Velocidade do Jogo: ${velocidadeJogo} ms`, 20, 50);\n"
                         "            doc.text(`Score 1: ${score1}`, 20, 70);\n"
                         "            doc.text(`Score 2: ${score2}`, 20, 80);\n"
                         "            doc.text(`Score 3: ${score3}`, 20, 90);\n"
                         "            doc.text(`Observações: `, 20, 100);\n"
                         "\n"
                         "            // Handling Observações with text wrapping:\n"
                         "            const observacoesMaxWidth = 160; // Adjust this value (in points) as needed\n"
                         "            const observacoesLines = doc.splitTextToSize(observacoes, observacoesMaxWidth);\n"
                         "            let observacoesY = 110; // Starting Y position for observations\n"
                         "\n"
                         "            observacoesLines.forEach(line => {\n"
                         "                doc.text(line, 20, observacoesY);\n"
                         "                observacoesY += 10; // Adjust this value (line height) as needed\n"
                         "                if (observacoesY > 250) { // Check if we are going to a new page\n"
                         "                    doc.addPage();\n"
                         "                    observacoesY = 20; // reset Y position\n"
                         "                }\n"
                         "            });\n"
                         "\n"
                         "            doc.save(\"Informações_do_paciente_ou_aluno.pdf\");\n"
                         "        }\n"
                         "    </script>\n"
                         "</body>\n"
                         "</html>\n";

// Função de callback para processar requisições HTTP
static err_t http_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
  if (p == NULL) {
      // Cliente fechou a conexão
      tcp_close(tpcb);
      return ERR_OK;
  }

  // Processa a requisição HTTP
  char *request = (char *)p->payload;

  if (strstr(request, "GET /led/on")) {
      gpio_put(LED_PIN, 1);  // Liga o LED
  } else if (strstr(request, "GET /led/off")) {
      gpio_put(LED_PIN, 0);  // Desliga o LED
  }

  // Envia a resposta HTTP
  tcp_write(tpcb, HTTP_RESPONSE, strlen(HTTP_RESPONSE), TCP_WRITE_FLAG_COPY);

  // Libera o buffer recebido
  pbuf_free(p);
   // Importante: Fecha a conexão após enviar a resposta.
  tcp_close(tpcb);


  return ERR_OK;
}

// Callback de conexão: associa o http_callback à conexão
static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err) {
 tcp_recv(newpcb, http_callback);  // Associa o callback HTTP
 
  // Define um tempo limite para recepção de dados.
  tcp_setprio(newpcb, TCP_PRIO_MIN); // Define prioridade baixa (opcional)
 
  return ERR_OK;
}

// Função de setup do servidor TCP
static void start_http_server(void) {
  struct tcp_pcb *pcb = tcp_new();
  if (!pcb) {
      printf("Erro ao criar PCB\n");
      return;
  }

  // Define o endereço IP estático
  ip_addr_t ip, gw, mask;
  IP4_ADDR(&ip, 192, 168, 125, 30); // IP estático  <-- Corrigido para o seu IP
  IP4_ADDR(&gw, 192, 168, 125, 1);  // Gateway (geralmente o roteador) <-- Coloque seu gateway correto
  IP4_ADDR(&mask, 255, 255, 255, 0); // Máscara de sub-rede

   // Desativa o DHCP (importante!)
  dhcp_stop(netif_default);

  // Configura o endereço IP estático, gateway e máscara de sub-rede na interface de rede
  netif_set_addr(netif_default, &ip, &mask, &gw);

  // Liga o servidor na porta 80
  if (tcp_bind(pcb, &ip, 80) != ERR_OK) {
      printf("Erro ao ligar o servidor na porta 80\n");
       tcp_close(pcb); // Fecha o pcb em caso de erro
      return;
  }

  pcb = tcp_listen(pcb);  // Coloca o PCB em modo de escuta
  tcp_accept(pcb, connection_callback);  // Associa o callback de conexão

  printf("Servidor HTTP rodando na porta 80...\n");
}*/

struct pixel_t {
  uint8_t G, R, B;
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t;

npLED_t leds[LED_COUNT];

PIO np_pio;
uint sm;

void npInit(uint pin) {
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0) {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true);
  }
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
  for (uint i = 0; i < LED_COUNT; ++i) {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

void npClear() {
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

void npWrite() {
  for (uint i = 0; i < LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100);
}

int debounce_button(int pin) {
    int estado_atual = gpio_get(pin);
    sleep_ms(DEBOUNCE_DELAY);
    int estado_final = gpio_get(pin);
    return estado_atual == estado_final ? estado_atual : -1;
}

int gerar_numero_aleatorio_1_a_4() {
    return (rand() % 4) + 1;
}

// Definição de uma função para inicializar o PWM no pino do buzzer
void pwm_init_buzzer(uint pin, int frequency) {  // Agora recebe a frequência
  gpio_set_function(pin, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(pin);

  pwm_config config = pwm_get_default_config();

  // Usa a frequência passada como parâmetro
  float clkdiv = (float)clock_get_hz(clk_sys) / (4096.0f * (float)frequency);
  if (clkdiv < 1.0f) {
      clkdiv = 1.0f;
  }
  uint8_t clkdiv_int = (uint8_t)clkdiv;
  uint8_t clkdiv_frac = (uint8_t)((clkdiv - clkdiv_int) * 256.0f);
  pwm_config_set_clkdiv_int_frac(&config, clkdiv_int, clkdiv_frac);

  pwm_init(slice_num, &config, true);
  pwm_set_gpio_level(pin, 0);
}

// Definição de uma função para emitir um beep com duração especificada
void beep(uint pin, uint duration_ms, int frequency) { // Recebe a frequência
  pwm_init_buzzer(pin, frequency); // Reconfigura o PWM com a nova frequência
  pwm_set_gpio_level(pin, 2048);
  sleep_ms(duration_ms);
  pwm_set_gpio_level(pin, 0);
  sleep_ms(100);
}

void gerar_padrao() {
    concatenacao = 0; // Reinicializa a concatenação
    for(int conjunto_numerico = 0; conjunto_numerico <= 8; conjunto_numerico++) {
        sleep_ms(100);
        int numero_aleatorio = gerar_numero_aleatorio_1_a_4();
        printf("%d", numero_aleatorio);

        if(concatenacao == 0) {
            concatenacao = numero_aleatorio;
        } else {
            concatenacao = concatenacao * 10 + numero_aleatorio;
        }
    }
}

void separar_padrao() {
    int numero = concatenacao;
    int total_digitos = 0;

    while (numero > 0 && total_digitos < 9) {
        digitos[total_digitos] = numero % 10;
        numero /= 10;
        total_digitos++;
    }

    for (int i = 0; i < total_digitos / 2; i++) {
        int temp = digitos[i];
        digitos[i] = digitos[total_digitos - i - 1];
        digitos[total_digitos - i - 1] = temp;
    }
}

int ler_posicao_joystick() {
    adc_select_input(0);
    uint16_t adc_x_raw = adc_read();
    adc_select_input(1);
    uint16_t adc_y_raw = adc_read();

    float x = (float)adc_x_raw / 4095.0;
    float y = (float)adc_y_raw / 4095.0;

    if (x < 0.3) { 
      npSetLED(2, 0, 0, 100);
      npWrite();
      sleep_ms(10);
      npClear();
      npWrite();
      sleep_ms(10);
      return 1; // BAIXO
    } else if (x > 0.7) {
        npSetLED(22, 0, 100, 100);
        npWrite();
        sleep_ms(10);
        npClear();
        npWrite();
        sleep_ms(10);
        return 4; // CIMA
    } else if (y < 0.3) {
        npSetLED(14, 100, 0, 0);
        npWrite();
        sleep_ms(10);
        npClear();
        npWrite();
        sleep_ms(10);
        return 3; // ESQUERDA
    } else if (y > 0.7) {
        npSetLED(10, 0, 100, 0);
        npWrite();
        sleep_ms(10);
        npClear();
        npWrite();
        sleep_ms(10);
        return 2; // DIREITA
    } else {
        return 0; // Centro (não usado para o jogo)
    }
}



// Função para configurar o PWM (chame uma vez no início do seu programa)
void setup_pwm(uint gpio_pin) {
    // Configura o pino como função PWM
    gpio_set_function(gpio_pin, GPIO_FUNC_PWM);

    // Obtém a "slice" PWM associada a este pino
    uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

    // Configura um "wrap" (valor máximo do contador PWM)
    // Isso define a frequência.  Um valor menor aumenta a frequência.
    // Um bom valor inicial é 255 (para ter 256 níveis de brilho).
    pwm_set_wrap(slice_num, 255);

    // Inicia o PWM com duty cycle 0 (desligado)
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(gpio_pin), 0);

    // Habilita o PWM na slice
    pwm_set_enabled(slice_num, true);
}

// Função para definir o brilho do LED (0-255)
void set_led_brightness(uint gpio_pin, uint16_t brightness) {
    // Obtém a slice PWM associada ao pino
    uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

    // Garante que o brilho esteja dentro dos limites (0-255)
    if (brightness > 255) {
        brightness = 255;
    }

    // Define o nível do canal (duty cycle)
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(gpio_pin), brightness);
}

void acerto_de_sequencia() {
  beep(BUZZER_PIN, 100, 120000); // Frequência alta para acerto
  set_led_brightness(LEDG_PIN, 12);
  sleep_ms(50);
  beep(BUZZER_PIN, 100, 120000); // Apenas um beep é suficiente
  set_led_brightness(LEDG_PIN, 0);
}

void erro_de_sequencia() {
  beep(BUZZER_PIN, 100, 2000);  // Frequência baixa para erro
  set_led_brightness(LEDR_PIN, 12);
  sleep_ms(50);
  beep(BUZZER_PIN, 100, 2000);  // Apenas um beep é suficiente
  set_led_brightness(LEDR_PIN, 0);
}

void verificar_sequencia_por_joystick(int indice) {
  while(true) {


      int posicao_joystick = ler_posicao_joystick();
      if (digitos[indice] == posicao_joystick) {
          printf("Você acertou a sequência %d!\n", indice+1);
          tentativas++;
          status = tentativas + 1;
          break;
      } else if (posicao_joystick != 0) {
          printf("Você errou a sequência %d!\n", indice+1);
          erro_de_sequencia();
          tentativas = 0;
          status = 0;
          return;
      }
      sleep_ms(20);
  }
  while (ler_posicao_joystick() != 0) {
      sleep_ms(20);
  }
}

void primeira_tentativa() {
    uint64_t start_time = to_ms_since_boot(get_absolute_time());
    int botao_correto = 0;
    primeiro_digito = digitos[0];

    while (to_ms_since_boot(get_absolute_time()) - start_time < 10000) {
        
      if(gpio_get(BTN_B) == 0){
        sleep_ms(3000);
        if(gpio_get(BTN_B) == 0){
          beep(BUZZER_PIN, 2000, 64000);  // Apenas um beep é suficiente
        }
      }
        int posicao_joystick = ler_posicao_joystick();
        if (primeiro_digito == posicao_joystick) {
            botao_correto = 1;
        }
        if (posicao_joystick != 0 && primeiro_digito != posicao_joystick) {
            tentativas = 0;
            printf("Você não pressionou o botão correto! \n");
            erro_de_sequencia();
            break;
        }
        if (botao_correto) {
            printf("Você acertou! \n");
            tentativas++;
            break;
        }
        sleep_ms(100);
    }
    while (ler_posicao_joystick() != 0) {
        sleep_ms(100);
    }
}

void piscarLED(int digito) {
  if (digito == 1) {
    npSetLED(2, 0, 0, 100);
  } else if (digito == 2) {
    npSetLED(10, 0, 100, 0);
  } else if (digito == 3) {
    npSetLED(14, 100, 0, 0);
  } else if (digito == 4) {
    npSetLED(22, 0, 100, 100);
  }
  npWrite();
  sleep_ms(dificuldade_por_tempo);
  npClear();
  npWrite();
  sleep_ms(dificuldade_por_tempo);
}

void contagem_regressiva(){

  // Numero 3
  npSetLED(1, 100, 0, 0);
  npSetLED(2, 100, 0, 0);
  npSetLED(3, 100, 0, 0);
  npSetLED(8, 100, 0, 0);
  npSetLED(11, 100, 0, 0);
  npSetLED(12, 100, 0, 0);
  npSetLED(18, 100, 0, 0);
  npSetLED(21, 100, 0, 0);
  npSetLED(22, 100, 0, 0);
  npSetLED(23, 100, 0, 0);
  npWrite();
  sleep_ms(500);
  npClear();
  npWrite();
  sleep_ms(500);
  // Numero 2
  npSetLED(1, 100, 0, 0);
  npSetLED(2, 100, 0, 0);
  npSetLED(3, 100, 0, 0);
  npSetLED(6, 100, 0, 0);
  npSetLED(12, 100, 0, 0);
  npSetLED(18, 100, 0, 0);
  npSetLED(21, 100, 0, 0);
  npSetLED(22, 100, 0, 0);
  npSetLED(23, 100, 0, 0);
  npWrite();
  sleep_ms(500);
  npClear();
  npWrite();
  sleep_ms(500);
  // Numero 1
  npSetLED(1, 100, 0, 0);
  npSetLED(2, 100, 0, 0);
  npSetLED(3, 100, 0, 0);
  npSetLED(7, 100, 0, 0);
  npSetLED(12, 100, 0, 0);
  npSetLED(17, 100, 0, 0);
  npSetLED(16, 100, 0, 0);
  npSetLED(22, 100, 0, 0);
  npWrite();
  sleep_ms(500);
  npClear();
  npWrite();
  sleep_ms(500);

}


int main() {

  
    // Inicializa a comunicação serial para depuração
    stdio_init_all();

    /*
    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
      start_column : 0,
      end_column : ssd1306_width - 1,
      start_page : 0,
      end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area); // Precisa dessa função para limpar a aréa do onled

    // zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    ssd1306_draw_string(ssd, 5, 5, "Iniciando.");

    render_on_display(ssd, &frame_area);

    // Inicializa o Wi-Fi
    if (cyw43_arch_init()) {
        printf("Erro ao inicializar o Wi-Fi\n");
        return 1;
    }

    // Habilita o modo station (conectar a um ponto de acesso)
    cyw43_arch_enable_sta_mode();

    // Loop para tentar conectar ao Wi-Fi repetidamente
    while (true) {
        printf("Conectando ao Wi-Fi...\n");

        // Tenta conectar ao Wi-Fi
        if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
            printf("Falha ao conectar ao Wi-Fi. Tentando novamente...\n");
            sleep_ms(5000); // Aguarda 5 segundos antes de tentar novamente
        } else {
            printf("Conectado ao Wi-Fi!\n");

            // Exibe o endereço IP atribuído
            // NÃO USA ISSO AQUI para configurar o IP estático.
            //uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
            //printf("Endereço IP: %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);

            break; // Sai do loop quando a conexão é bem-sucedida
        }
    }
    */
    

    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);

    gpio_init(LEDG_PIN);
    gpio_set_dir(LEDG_PIN, GPIO_OUT);
    setup_pwm(LEDG_PIN); 
    gpio_init(LEDR_PIN);
    gpio_set_dir(LEDR_PIN, GPIO_OUT);
    setup_pwm(LEDR_PIN);
    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_B);
    npInit(LED_PIN);

    // Configuração do GPIO para o buzzer como saída
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);

    /* Inicia o servidor HTTP *após* conectar ao Wi-Fi
    start_http_server();*/
  

    
    while (true) {

        printf("Endereço IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list))); //Mostra o IP.

        if(gpio_get(BTN_B) == 0) {
          // Debounce
          sleep_ms(20);
          if(gpio_get(BTN_B) == 0) {
            
            // Reinicializa a semente do gerador de números aleatórios
            srand(to_ms_since_boot(get_absolute_time()));

            // Gera um novo padrão de números aleatórios
            gerar_padrao();
            separar_padrao();

            int posicao_joystick = ler_posicao_joystick();

            if (tentativas == 0) { printf("\nInício \n"); contagem_regressiva(); piscarLED(digitos[0]); primeira_tentativa(); }
            if (tentativas == 1) { acerto_de_sequencia(); piscarLED(digitos[0]); piscarLED(digitos[1]); primeira_tentativa(); }
            if (tentativas == 2) { verificar_sequencia_por_joystick(1); }
            if (tentativas == 3) { acerto_de_sequencia(); piscarLED(digitos[0]); piscarLED(digitos[1]); piscarLED(digitos[2]); primeira_tentativa(); }
            if (tentativas == 4) { verificar_sequencia_por_joystick(1); } if (tentativas == 5) { verificar_sequencia_por_joystick(2); }
            if (tentativas == 6) { acerto_de_sequencia(); piscarLED(digitos[0]); piscarLED(digitos[1]); piscarLED(digitos[2]); piscarLED(digitos[3]); primeira_tentativa(); }
            if (tentativas == 7) { verificar_sequencia_por_joystick(1); } if (tentativas == 8) { verificar_sequencia_por_joystick(2); } if (tentativas == 9) { verificar_sequencia_por_joystick(3); }
            if (tentativas == 10) { acerto_de_sequencia(); piscarLED(digitos[0]); piscarLED(digitos[1]); piscarLED(digitos[2]); piscarLED(digitos[3]); piscarLED(digitos[4]); primeira_tentativa(); }
            if (tentativas == 11) { verificar_sequencia_por_joystick(1); } if (tentativas == 12) { verificar_sequencia_por_joystick(2); }
            if (tentativas == 13) { verificar_sequencia_por_joystick(3); } if (tentativas == 14) { verificar_sequencia_por_joystick(4); }
            if (tentativas == 15) { acerto_de_sequencia(); piscarLED(digitos[0]); piscarLED(digitos[1]); piscarLED(digitos[2]); piscarLED(digitos[3]); piscarLED(digitos[4]); piscarLED(digitos[5]); primeira_tentativa(); }
            if (tentativas == 16) { verificar_sequencia_por_joystick(1); } if (tentativas == 17) { verificar_sequencia_por_joystick(2); }
            if (tentativas == 18) { verificar_sequencia_por_joystick(3); } if (tentativas == 19) { verificar_sequencia_por_joystick(4); } if (tentativas == 20) { verificar_sequencia_por_joystick(5); }
            if (tentativas == 21) { acerto_de_sequencia(); piscarLED(digitos[0]); piscarLED(digitos[1]); piscarLED(digitos[2]); piscarLED(digitos[3]); piscarLED(digitos[4]); piscarLED(digitos[5]); piscarLED(digitos[6]); primeira_tentativa(); }
            if (tentativas == 22) { verificar_sequencia_por_joystick(1); } if (tentativas == 23) { verificar_sequencia_por_joystick(2); }
            if (tentativas == 24) { verificar_sequencia_por_joystick(3); } if (tentativas == 25) { verificar_sequencia_por_joystick(4); }
            if (tentativas == 26) { verificar_sequencia_por_joystick(5); } if (tentativas == 27) { verificar_sequencia_por_joystick(6); }
            if (tentativas == 28) { acerto_de_sequencia(); piscarLED(digitos[0]); piscarLED(digitos[1]); piscarLED(digitos[2]); piscarLED(digitos[3]); piscarLED(digitos[4]); piscarLED(digitos[5]); piscarLED(digitos[6]); piscarLED(digitos[7]); primeira_tentativa(); }
            if (tentativas == 29) { verificar_sequencia_por_joystick(1); } if (tentativas == 30) { verificar_sequencia_por_joystick(2); }
            if (tentativas == 31) { verificar_sequencia_por_joystick(3); } if (tentativas == 32) { verificar_sequencia_por_joystick(4); }
            if (tentativas == 33) { verificar_sequencia_por_joystick(5); } if (tentativas == 34) { verificar_sequencia_por_joystick(6); } if (tentativas == 35) { verificar_sequencia_por_joystick(7); }
            if (tentativas == 36) { acerto_de_sequencia(); piscarLED(digitos[0]); piscarLED(digitos[1]); piscarLED(digitos[2]); piscarLED(digitos[3]); piscarLED(digitos[4]); piscarLED(digitos[5]); piscarLED(digitos[6]); piscarLED(digitos[7]); piscarLED(digitos[8]); primeira_tentativa(); }
            if (tentativas == 37) { verificar_sequencia_por_joystick(1); } if (tentativas == 38) { verificar_sequencia_por_joystick(2); }
            if (tentativas == 39) { verificar_sequencia_por_joystick(3); } if (tentativas == 40) { verificar_sequencia_por_joystick(4); }
            if (tentativas == 41) { verificar_sequencia_por_joystick(5); } if (tentativas == 42) { verificar_sequencia_por_joystick(6); }
            if (tentativas == 43) { verificar_sequencia_por_joystick(7); } if (tentativas == 44) { verificar_sequencia_por_joystick(8); }

          }
        }

        sleep_ms(100);
    }

    return 0;
}
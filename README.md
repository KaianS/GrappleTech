# 🥋 GrappleTech

GrappleTech é um sistema embarcado inovador projetado para monitoramento e análise de treinos de jiu-jitsu. Utilizando sensores de força e detecção de quedas, o sistema fornece feedback em tempo real sobre a intensidade do treino e registra eventos importantes durante a prática.

![GrappleTech Demo](bjj-jiu-jitsu.gif)

## ✨ Características

- 💪 Monitoramento de força em dois eixos
- 🎯 Detecção automática de quedas
- 💓 Simulação de batimentos cardíacos baseada na intensidade
- 🏆 Sistema de pontuação para competições
- 📊 Interface visual via display OLED
- 🌈 Feedback visual através de matriz LED RGB
- 🔊 Alertas sonoros configuráveis
- 📡 Comunicação serial para registro de dados

## 🔧 Hardware Necessário

- Raspberry Pi Pico
- Display OLED SSD1306
- Matriz LED WS2812B 5x5
- Microfone
- Sensores de força (2x)
- Buzzer
- Botões (3x)
- Cabos e conectores necessários

## 🛠️ Configuração do Ambiente

1. Instale o VSCode
2. Configure o ambiente para placa BitDogLab
3. Clone este repositório:
```bash
git clone https://github.com/kaians/grappletech.git
```

4. Acesse o repositório clonado, compile e envie o código para placa.
5. (opcional) Configure e clone o repositório para simulação UART.
```bash
git clone https://github.com/kaians/grappletech.git
```

## 📁 Estrutura do Projeto

```
grappletech/
├── src/
│   ├── buzzer.c
│   ├── buzzer.h
│   ├── config.h
│   ├── display.c
│   ├── display.h
│   ├── hardware.c
│   ├── hardware.h
│   ├── leds.c
│   ├── leds.h
│   ├── main.c
│   ├── sensores.c
│   ├── sensores.h
│   ├── treino.c
│   ├── treino.h
├── ssd1306.c
├── ssd1306.h 
├── ws2818b.pio

```

## Vídeo Demonstração

[![Watch the video](https://img.youtube.com/vi/vhW9ee2U6Go/maxresdefault.jpg)](https://youtu.be/vhW9ee2U6Go)

### [Vídeo Demonstração GrappleTech](https://youtu.be/vhW9ee2U6Go)

```

🎮 Uso

1. Calibração Inicial
   - Ligue o sistema
   - Aguarde a calibração automática dos sensores (5 segundos)

2. Início do Treino
   - Pressione o botão START para iniciar
   - O display mostrará as informações em tempo real

3. Durante o Treino
   - Monitore a intensidade através da matriz LED
   - Observe os batimentos simulados no display
   - Use os botões A e B para registrar pontos

4. Finalização
   - Pressione START novamente para encerrar
   - Os dados serão enviados via serial

🎯 Funcionalidades

Sistema de Força
- Medição contínua em dois eixos
- Calibração automática
- Escala de 0-4095 para cada eixo

Detecção de Quedas
- Monitoramento via microfone
- Threshold configurável
- Debounce de 1 segundo
- Feedback visual e sonoro

Simulação de Batimentos
- Range: 40-180 BPM
- Ajuste baseado na intensidade
- Decaimento automático
- Alertas de limite superior

Interface Visual
- Display OLED:
  - Tempo de treino
  - BPM atual
  - Força aplicada
  - Contagem de quedas
- Matriz LED:
  - Indicador de intensidade
  - Feedback de eventos
```

## 🤝 Contribuição

Contribuições são bem-vindas! Por favor, leia nosso guia de contribuição antes de submeter pull requests.

1. Faça o fork do projeto
2. Crie sua feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit suas mudanças (`git commit -m 'Add some AmazingFeature'`)
4. Push para a branch (`git push origin feature/AmazingFeature`)
5. Abra um Pull Request



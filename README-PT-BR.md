
<p align="center"><img src="buildroot/share/pixmaps/logo/marlin-outrun-nf-500.png" height="250" alt="Logo do MarlinFirmware" /></p>

<h1 align="center">Firmware de Impressora 3D Marlin</h1>

<p align="center">
    <a href="/LICENSE"><img alt="Licença GPL-V3.0" src="https://img.shields.io/github/license/marlinfirmware/marlin.svg"></a>
    <a href="https://github.com/MarlinFirmware/Marlin/graphs/contributors"><img alt="Contribuidores" src="https://img.shields.io/github/contributors/marlinfirmware/marlin.svg"></a>
    <a href="https://github.com/MarlinFirmware/Marlin/releases"><img alt="Data do Último Lançamento" src="https://img.shields.io/github/release-date/MarlinFirmware/Marlin"></a>
    <a href="https://github.com/MarlinFirmware/Marlin/actions/workflows/ci-build-tests.yml"><img alt="Status do CI" src="https://github.com/MarlinFirmware/Marlin/actions/workflows/ci-build-tests.yml/badge.svg"></a>
    <a href="https://github.com/sponsors/thinkyhead"><img alt="Patrocínios no GitHub" src="https://img.shields.io/github/sponsors/thinkyhead?color=db61a2"></a>
    <br />
    <a href="https://bsky.app/profile/marlinfw.org"><img alt="Siga marlinfw.org no Bluesky" src="https://img.shields.io/static/v1?label=&message=Siga @marlinfw.org&color=1185FE&logo=bluesky&logoColor=white"></a>
    <a href="https://fosstodon.org/@marlinfirmware"><img alt="Siga MarlinFirmware no Mastodon" src="https://img.shields.io/mastodon/follow/109450200866020466?domain=https%3A%2F%2Ffosstodon.org&logoColor=%2300B&style=social"></a>
</p>

Documentação adicional pode ser encontrada na [Página Inicial do Marlin](//marlinfw.org/).  
Por favor, teste este firmware e nos avise se encontrar algum problema. Voluntários estão prontos para ajudar!

## Branch de Correções do Marlin 2.1

__Não é para uso em produção. Use com cautela!__

O Marlin 2.1 continua oferecendo suporte a placas ARM 32 bits e AVR 8 bits, além de adicionar suporte para até 9 eixos coordenados e até 8 extrusoras.

Este branch é para correções da versão mais recente 2.1.x. Periodicamente, ele servirá de base para o próximo lançamento menor da linha 2.1.x.

Versões anteriores do Marlin podem ser baixadas na [página de lançamentos](//github.com/MarlinFirmware/Marlin/releases).

## Configurações de Exemplo

Antes de compilar o Marlin para sua máquina, você precisará de uma configuração específica para o seu hardware. Ao solicitar, seu fornecedor deve fornecer o código-fonte completo e as configurações da sua máquina. No entanto, se quiser instalar uma versão mais recente do Marlin, você precisará de arquivos de configuração atualizados. Felizmente, a comunidade do Marlin já contribuiu com dezenas de configurações testadas para ajudar no início. Visite o repositório [MarlinFirmware/Configurations](//github.com/MarlinFirmware/Configurations) para encontrar a configuração mais próxima da sua impressora.

## Compilando o Marlin 2.1

Para compilar e enviar o Marlin você pode usar uma destas ferramentas:

- O [Visual Studio Code](//code.visualstudio.com/download) com a extensão [Auto Build Marlin](//marlinfw.org/docs/basics/auto_build_marlin.html).
- A [IDE do Arduino](//www.arduino.cc/en/main/software): Veja [Compilando Marlin com Arduino](//marlinfw.org/docs/basics/install_arduino.html).
- Também é possível usar VSCode com devcontainer: Veja [Instalando Marlin (VSCode devcontainer)](http://marlinfw.org/docs/basics/install_devcontainer_vscode.html).

O Marlin é otimizado para ser compilado com a extensão **PlatformIO IDE** no **Visual Studio Code**. Ainda é possível compilar com a **IDE do Arduino**, e temos planos para melhorar essa experiência, mas por enquanto o PlatformIO é a melhor escolha.

## Placas AVR 8 Bits

Pretendemos continuar oferecendo suporte às placas AVR 8 bits indefinidamente, mantendo uma base de código única que possa ser aplicada a todas as máquinas. Queremos que hobbystas, experimentadores e donos de máquinas antigas também se beneficiem das inovações da comunidade tanto quanto os donos de equipamentos mais modernos. Além disso, essas máquinas baseadas em AVR costumam ser ideais para testes e feedbacks!

## Camada de Abstração de Hardware (HAL)

O Marlin inclui uma camada de abstração de hardware para portar o firmware para uma grande variedade de chips. Essa camada trata das diferenças entre chips de forma modular, permitindo que as funcionalidades do Marlin sejam aproveitadas ao máximo.

## Licença

Marlin é publicado sob a licença GPL, então você pode usar, redistribuir e modificar o código-fonte, desde que o código derivado também seja publicado sob a mesma licença. Consulte o arquivo [LICENSE](https://github.com/MarlinFirmware/Marlin/blob/bugfix-2.1.x/LICENSE) para mais detalhes.

## Ajude o Marlin!

Você pode ajudar o projeto Marlin contribuindo com código, traduções, testes ou apoiando financeiramente no [GitHub Sponsors](https://github.com/sponsors/thinkyhead).

...Marlin para diferentes plataformas de hardware. O HAL define as interfaces entre o núcleo do Marlin e o hardware da plataforma. O Marlin suporta atualmente as seguintes arquiteturas:

- AVR
- SAM (Arduino Due)
- SAMD (Arduino Zero, etc.)
- STM32F1, STM32F4, STM32F7, STM32H7
- LPC176x (Smoothieboard, ReARM, Archim, MKS Sbase, etc.)
- Teensy 3.5 e 3.6 (ARM Cortex-M4)
- ESP32 (experimental)

## Comunicação Serial

- **Baudrates suportados:** 250000, 115200, 57600, 38400, 19200, 9600
- O baudrate padrão é 250000 para maior velocidade e estabilidade

## Atualizações e Contribuições

Aceitamos correções de bugs, melhorias de desempenho e novas funcionalidades. Veja as instruções de contribuição na [Wiki do Marlin](https://github.com/MarlinFirmware/Marlin/wiki/Contributing).

## Licença

Marlin é um software livre licenciado sob a [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.html). Você pode redistribuí-lo e/ou modificá-lo sob os termos da GPL. Para mais detalhes, veja o arquivo [LICENSE](LICENSE).

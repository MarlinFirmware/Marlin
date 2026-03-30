# Прошивка Marlin с Linux-сервера (Repetier Host)

Подробная инструкция для Anet A8 / ATmega1284P (Sanguino), когда прошивка собирается как `firmware.hex` и заливается с Linux-сервера.

## Что потребуется

- Linux-сервер с доступом к принтеру по USB.
- Файл прошивки `firmware.hex` (у вас обычно: `.pio/build/sanguino1284p/firmware.hex`).
- Установленный `avrdude`.
- Понимание, как вы шьете:
  - через USB загрузчик (bootloader), или
  - через ISP-программатор (например, USBasp), если загрузчика нет.

## 1. Подготовка сервера

### 1.1 Установить avrdude

Для Debian/Ubuntu/Raspberry Pi OS:

```bash
sudo apt update
sudo apt install avrdude
```

Проверка:

```bash
avrdude -v
```

### 1.2 Найти USB-порт принтера

Подключите принтер и проверьте:

```bash
ls -l /dev/ttyUSB* /dev/ttyACM*
dmesg | tail -n 50
```

Обычно порт выглядит как `/dev/ttyUSB0` или `/dev/ttyACM0`.

### 1.3 Остановить сервис, который держит порт

Если запущен Repetier-Server (или OctoPrint), он блокирует порт во время прошивки.

Примеры:

```bash
sudo systemctl stop repetier-server
```

или

```bash
sudo systemctl stop octoprint
```

Проверка, кто держит порт:

```bash
sudo lsof /dev/ttyUSB0
```

Если процесс еще есть, завершите его перед прошивкой.

## 2. Где взять файл прошивки

После сборки Marlin для Sanguino файл обычно находится здесь:

```text
.pio/build/sanguino1284p/firmware.hex
```

Перед прошивкой убедитесь, что файл существует:

```bash
ls -lh .pio/build/sanguino1284p/firmware.hex
```

## 3. Способ A: Прошивка через USB загрузчик

Этот способ работает, если загрузчик уже установлен в контроллере.

### 3.1 Базовая команда

Если у вас действительно Optiboot, сначала используйте `arduino` + `115200`:

```bash
avrdude -p atmega1284p -c arduino -P /dev/ttyUSB0 -b 115200 -D -U flash:w:.pio/build/sanguino1284p/firmware.hex:i
```

Что означают ключи:

- `-p atmega1284p` — тип МК.
- `-c arduino` — протокол для Optiboot (через auto-reset по DTR).
- `-P /dev/ttyUSB0` — порт.
- `-b 115200` — типовая скорость Optiboot.
- `-D` — не стирать EEPROM через чип erase.
- `-U flash:w:...:i` — записать hex в flash.

### 3.2 Если не прошивается

Для старых/альтернативных загрузчиков (не Optiboot) используйте fallback-команду:

```bash
avrdude -p atmega1284p -c stk500v1 -P /dev/ttyUSB0 -b 115200 -D -U flash:w:.pio/build/sanguino1284p/firmware.hex:i
```

Если не помогло, попробуйте 57600:

```bash
avrdude -p atmega1284p -c stk500v1 -P /dev/ttyUSB0 -b 57600 -D -U flash:w:.pio/build/sanguino1284p/firmware.hex:i
```

Если ошибки синхронизации сохраняются (`not in sync`), вероятно нет загрузчика или неверный протокол.

## 4. Способ B: Прошивка через USBasp (ISP)

Используйте этот вариант, если:

- загрузчика нет,
- загрузчик поврежден,
- USB-прошивка не проходит.

### 4.1 Подключение

Подключите USBasp к ISP-пинам платы (MISO, MOSI, SCK, RESET, VCC, GND) согласно распиновке вашей версии платы.

### 4.2 Проверка связи

```bash
avrdude -c usbasp -p atmega1284p -v
```

Если устройство найдено, можно шить.

### 4.3 Запись прошивки

```bash
avrdude -c usbasp -p atmega1284p -U flash:w:.pio/build/sanguino1284p/firmware.hex:i
```

При необходимости медленной прошивки (нестабильная связь):

```bash
avrdude -c usbasp -p atmega1284p -B 10 -U flash:w:.pio/build/sanguino1284p/firmware.hex:i
```

## 5. После прошивки

### 5.1 Запустить сервис обратно

```bash
sudo systemctl start repetier-server
```

или

```bash
sudo systemctl start octoprint
```

### 5.2 Проверить, что прошивка поднялась

Подключитесь к принтеру и отправьте:

- `M115` — версия и возможности прошивки.
- `M503` — текущие настройки.

Если включен EEPROM, после новой прошивки полезно:

1. `M502` (загрузить дефолты из прошивки)
2. `M500` (сохранить в EEPROM)
3. `M503` (проверить)

## 6. Быстрый чек-лист

1. Собран актуальный `firmware.hex`.
2. Установлен `avrdude`.
3. Определен правильный порт (`/dev/ttyUSB0` и т.п.).
4. Остановлен Repetier/OctoPrint на время прошивки.
5. Выбран правильный способ:
   - bootloader через USB, или
   - ISP через USBasp.
6. Выполнена прошивка без ошибок.
7. Хост-сервис запущен обратно.
8. Проверены `M115` и базовые движения/нагрев.

## 7. Частые ошибки и решения

### `avrdude: ser_open(): can't open device /dev/ttyUSB0`

- Неверный порт.
- Нет прав на устройство.
- Порт занят сервисом.

Проверить права/группу:

```bash
groups
ls -l /dev/ttyUSB0
```

Обычно пользователь должен быть в группе `dialout`:

```bash
sudo usermod -aG dialout $USER
```

После этого перелогиньтесь.

### `stk500_getsync()` / `not in sync`

- Неверная скорость `-b`.
- Неверный протокол `-c`.
- Нет загрузчика.
- Автосброс не срабатывает.

Решение: попробовать альтернативную скорость, затем перейти на USBasp.

### `initialization failed, rc=-1` (с USBasp)

- Неверное подключение ISP.
- Не подано питание на плату.
- Перепутаны MISO/MOSI/SCK/RESET.

Проверить проводку и попробовать медленнее (`-B 10`).

## 8. Рекомендации по безопасности

- Не отключайте питание во время записи flash.
- Прошивайте при стабильном USB-кабеле и питании.
- После прошивки сначала проверьте концевики и направление осей, потом нагрев.
- Если менялись критические параметры движения/термисторов, проверьте их до печати.

## 9. Готовые шаблоны команд

### Через загрузчик (USB)

```bash
PORT=/dev/ttyUSB0
HEX=.pio/build/sanguino1284p/firmware.hex

sudo systemctl stop repetier-server
avrdude -p atmega1284p -c arduino -P "$PORT" -b 115200 -D -U flash:w:"$HEX":i
sudo systemctl start repetier-server
```

### Через USBasp

```bash
HEX=.pio/build/sanguino1284p/firmware.hex

sudo systemctl stop repetier-server
avrdude -c usbasp -p atmega1284p -U flash:w:"$HEX":i
sudo systemctl start repetier-server
```

## 10. Подробная настройка OctoPrint + Firmware Updater

Ниже пошаговая схема для прошивки через веб-интерфейс OctoPrint.

### 10.1 Что должно быть готово заранее

1. OctoPrint видит принтер и может подключаться к нему.
2. В окружении OctoPrint установлен `avrdude`.
3. Вы знаете порт принтера (`/dev/ttyUSB0` или `/dev/ttyACM0`).
4. Под рукой есть готовый `firmware.hex`.

### 10.2 Установка плагина Firmware Updater

1. Откройте `Settings` в OctoPrint.
2. Перейдите в `Plugin Manager`.
3. Нажмите `Get More...`.
4. Найдите `Firmware Updater`.
5. Нажмите `Install`.
6. После установки перезапустите OctoPrint.

### 10.3 Проверка `avrdude` (обязательно)

Плагин только вызывает внешнюю команду, сам прошивать без `avrdude` не умеет.

Проверьте в системе хоста:

```bash
avrdude -v
```

Если OctoPrint установлен в Docker, проверяйте внутри контейнера:

```bash
docker exec -it octoprint avrdude -v
```

Если команды нет, установите `avrdude` в образ/контейнер.

### 10.4 Важный момент про Docker

Если OctoPrint в Docker, пробросьте USB-устройство принтера в контейнер.

Пример для `docker-compose.yml`:

```yaml
services:
  octoprint:
    image: octoprint/octoprint:latest
    container_name: octoprint
    restart: unless-stopped
    ports:
      - "5000:80"
    devices:
      - "/dev/ttyUSB0:/dev/ttyUSB0"
    group_add:
      - dialout
    volumes:
      - ./octoprint:/octoprint
```

Если у вас принтер как `/dev/ttyACM0`, укажите его вместо `/dev/ttyUSB0`.

### 10.5 Базовая настройка плагина (через команду)

В настройках Firmware Updater укажите команду прошивки.

Для Optiboot (рекомендуется начать с этого):

```bash
avrdude -p atmega1284p -c arduino -P {serial_port} -b 115200 -D -U flash:w:{firmware_file}:i
```

Если в вашей версии плагина имена шаблонов отличаются, возьмите placeholders из подсказок в интерфейсе плагина и подставьте их в ту же команду.

Fallback для не-Optiboot:

```bash
avrdude -p atmega1284p -c stk500v1 -P {serial_port} -b 115200 -D -U flash:w:{firmware_file}:i
```

### 10.6 Как безопасно запускать прошивку из плагина

Перед нажатием `Flash`:

1. Остановите печать и убедитесь, что очередь пуста.
2. Отключитесь от принтера в OctoPrint (`Disconnect`).
3. Загрузите `firmware.hex` в плагине.
4. Запустите прошивку.
5. Дождитесь завершения и переподключитесь.
6. Проверьте `M115`.

### 10.7 Рекомендуемый вариант: прошивка через скрипт

Более надежно вызвать не длинную команду, а скрипт, где можно логировать ошибки и добавлять fallback.

Пример `flash_marlin.sh`:

```bash
#!/usr/bin/env bash
set -euo pipefail

PORT="${1:-/dev/ttyUSB0}"
HEX="${2:?usage: flash_marlin.sh <port> <hex>}"

echo "[flash] Port: $PORT"
echo "[flash] Hex : $HEX"

# Попытка для Optiboot
if avrdude -p atmega1284p -c arduino -P "$PORT" -b 115200 -D -U flash:w:"$HEX":i; then
  echo "[flash] Done with arduino/115200"
  exit 0
fi

echo "[flash] Fallback to stk500v1/115200"
if avrdude -p atmega1284p -c stk500v1 -P "$PORT" -b 115200 -D -U flash:w:"$HEX":i; then
  echo "[flash] Done with stk500v1/115200"
  exit 0
fi

echo "[flash] Fallback to stk500v1/57600"
avrdude -p atmega1284p -c stk500v1 -P "$PORT" -b 57600 -D -U flash:w:"$HEX":i
echo "[flash] Done with stk500v1/57600"
```

Сделайте исполняемым:

```bash
chmod +x flash_marlin.sh
```

И укажите в плагине команду вида:

```bash
/path/to/flash_marlin.sh {serial_port} {firmware_file}
```

### 10.8 Диагностика проблем в плагине

1. `avrdude not found`
   - Установить `avrdude` в то же окружение, где работает OctoPrint.
2. `can't open device /dev/ttyUSB0`
   - Неверный порт, нет прав, порт не проброшен в Docker, порт занят.
3. `not in sync`
   - Неправильная комбинация протокол/скорость, нет загрузчика.
4. В Docker не виден порт
   - Проверить `devices:` в compose и права группы `dialout`.

После успешной прошивки в любом случае проверяйте:

1. `M115` — версия прошивки.
2. `M503` — параметры.
3. Базовые тесты осей и концевиков.

---

Если нужен, можно сделать отдельный короткий вариант "1 команда + 1 проверка" именно под ваш сервер и ваш порт.
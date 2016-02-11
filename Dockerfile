FROM ubuntu:14.04.3

RUN apt-get update && apt-get install -y make gcc-avr=1:4.8-2.1 avr-libc=1:1.8.0-4.1
WORKDIR /usr/src/myapp

ENV LANGUAGE_CHOICE='EN'
ENV HARDWARE='witbox_2,hephestos_2'
ENV WWW_DIR='/usr/src/build'
ENV ARDUINO='/usr/src/myapp/arduino'
ENV WORKSPACE='/usr/src/myapp'
ENV FW_DIR='/usr/src/myapp/firmware'
ENV CONFIG_DIR_DEST='/usr/src/myapp/firmware/Marlin'
ENV CONFIG_DIR='/usr/src/myapp/firmware/Marlin/config'

COPY ./entrypoint.sh /entrypoint.sh
RUN chmod +rx /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]

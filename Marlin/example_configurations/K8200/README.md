# Example Configuration for Vellemann [K8200](http://www.k8200.eu/)
* Configuration files for **Vellemann K8200** (with [VM8201](http://www.vellemanprojects.eu/products/view/?id=416158) - LCD Option for K8200)
* K8200 is a 3Drag clone - configuration should work with 3Drag http://reprap.org/wiki/3drag, too. Please report.

* updated manually with parameters from genuine Vellemann Firmware "firmware_k8200_marlinv2" based on the recent development branch

* VM8201 uses "DISPLAY_CHARSET_HD44870_JAPAN" and "ULTIMAKERCONTROLLER"
* german (de) translation with umlaut is supported now - thanks to @AnHardt for the great hardware based umlaut support

I [@CONSULitAS](https://github.com/CONSULitAS) tested the changes on my K8200 with 20x4-LCD and Arduino 1.6.12 for Mac (SD library added to IDE manually), 2016-11-18 - everything works well.

**Source for genuine [Vellemann Firmware](http://www.k8200.eu/support/downloads/)**
* V2.1.1 (for z axis upgrade, date branched: 2013-06-05): [firmware_k8200_v2.1.1.zip](http://www.k8200.eu/downloads/files/downloads/firmware_k8200_v2.1.1.zip)
  * see also https://github.com/CONSULitAS/Marlin-K8200/tree/Vellemann_firmware_k8200_v2.1.1.zip

* V2 (with LCD/SD-Support, date branched: 2013-06-05): [firmware_k8200_marlinv2.zip](http://www.k8200.eu/downloads/files/downloads/firmware_k8200_marlinv2.zip)
  * see also https://github.com/CONSULitAS/Marlin-K8200/tree/Vellemann_firmware_k8200_marlinv2.zip

* V1 (without LCD/SD-Support, date branched: 2012-10-02): [firmware_k8200_marlinv1.zip](http://www.k8200.eu/downloads/files/downloads/firmware_k8200_marlinv1.zip)
  * see also https://github.com/CONSULitAS/Marlin-K8200/tree/Vellemann_firmware_k8200_marlinv1.zip

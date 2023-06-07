#include <IRremote.h>  // Biblioteca necessária para emitir e receber sinais utilizando infravermelho
#include <EEPROM.h>    // Biblioteca necessária para escrever e ler na memória do arduino

void setup() {  // Declaração de entradas e saídas

  pinMode(5, INPUT);                                     // Botão 1
  pinMode(6, INPUT);                                     // Botão 2
  pinMode(7, INPUT);                                     // Botão 3
  pinMode(8, INPUT);                                     // Gravar/Emitir
  IrReceiver.begin(9, ENABLE_LED_FEEDBACK);              // Receptor infravermelho
  IrSender.begin(10, ENABLE_LED_FEEDBACK, LED_BUILTIN);  // LED infravermelho
}

void loop() {  // Parte lógica

  if (digitalRead(8) == 1) {  // Modo gravação

    if (IrReceiver.decode() && digitalRead(5) == 1) {  // Gravar em botão 1
      if (IrReceiver.decodedIRData.decodedRawData != 0)
        writeLongIntoEEPROM(0, IrReceiver.decodedIRData.decodedRawData);
      IrReceiver.resume();

    } else if (IrReceiver.decode() && digitalRead(6) == 1) {  // Gravar em botão 2
      if (IrReceiver.decodedIRData.decodedRawData != 0)
        writeLongIntoEEPROM(4, IrReceiver.decodedIRData.decodedRawData);
      IrReceiver.resume();

    } else if (IrReceiver.decode() && digitalRead(7) == 1) {  // Gravar em botão 3
      if (IrReceiver.decodedIRData.decodedRawData != 0)
        writeLongIntoEEPROM(8, IrReceiver.decodedIRData.decodedRawData);
      IrReceiver.resume();
    }

  } else {  // Modo emissão

    if (digitalRead(5) == 1) {  // Emite instrução gravada no botão 1
      IrSender.sendNECRaw(readLongFromEEPROM(0), 1);
      delay(100);
    } else if (digitalRead(6) == 1) {  // Emite instrução gravada no botão 2
      IrSender.sendNECRaw(readLongFromEEPROM(4), 1);
      delay(100);
    } else if (digitalRead(7) == 1) {  // Emite instrução gravada no botão 3
      IrSender.sendNECRaw(readLongFromEEPROM(8), 1);
      delay(100);
    }
  }
}

void writeLongIntoEEPROM(int address, long number) {  // Função para escrever um valor de 4 bytes na memória do Arduino.
  EEPROM.update(address, (number >> 24) & 0xFF);
  EEPROM.update(address + 1, (number >> 16) & 0xFF);
  EEPROM.update(address + 2, (number >> 8) & 0xFF);
  EEPROM.update(address + 3, number & 0xFF);
}
long readLongFromEEPROM(int address) {  // Função para ler um valor de 4 bytes na memória do Arduino.
  return ((long)EEPROM.read(address) << 24) + ((long)EEPROM.read(address + 1) << 16) + ((long)EEPROM.read(address + 2) << 8) + (long)EEPROM.read(address + 3);
}

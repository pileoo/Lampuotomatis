const int relayPin = 8;
const int ldrPin = A0;
const int pirPin = 2;

const int ldrThreshold = 300;

int pirState = LOW;

// Fungsi Keanggotaan Cahaya
float keanggotaanCahayaRendah(float nilaiCahaya) {
  if (nilaiCahaya <= 200) {
    return 1.0;
  } else if (nilaiCahaya > 200 && nilaiCahaya < 300) {
    return (300 - nilaiCahaya) / (300 - 200);
  } else {
    return 0.0;
  }
}

float keanggotaanCahayaTinggi(float nilaiCahaya) {
  if (nilaiCahaya >= 300) {
    return 1.0;
  } else if (nilaiCahaya > 200 && nilaiCahaya < 300) {
    return (nilaiCahaya - 200) / (300 - 200);
  } else {
    return 0.0;
  }
}

// Fungsi Keanggotaan Gerakan
float keanggotaanGerakan(int nilaiGerakan) {
  if (nilaiGerakan == HIGH) {
    return 1.0;
  } else {
    return 0.0;
  }
}

// Fungsi Keanggotaan Aksi Relay
float keanggotaanRelayOff(float nilaiRelay) {
  if (nilaiRelay >= 0 && nilaiRelay <= 50) {
    return 1.0;
  } else if (nilaiRelay > 50 && nilaiRelay < 100) {
    return (100 - nilaiRelay) / (100 - 50);
  } else {
    return 0.0;
  }
}

float keanggotaanRelayOn(float nilaiRelay) {
  if (nilaiRelay >= 50 && nilaiRelay <= 100) {
    return 1.0;
  } else if (nilaiRelay > 0 && nilaiRelay < 50) {
    return nilaiRelay / 50;
  } else {
    return 0.0;
  }
}

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(pirPin, INPUT);

  digitalWrite(relayPin, HIGH);

  Serial.begin(9600);
}

void loop() {
  int ldrValue = analogRead(ldrPin);
  pirState = digitalRead(pirPin);

  Serial.print("LDR: ");
  Serial.print(ldrValue);
  Serial.print(", PIR: ");
  Serial.println(pirState);
  Serial.print(", Relay: ");
  Serial.println(digitalRead(relayPin));

  // Inferensi Fuzzy
  float nilaiCahayaRendah = keanggotaanCahayaRendah(ldrValue);
  float nilaiCahayaTinggi = keanggotaanCahayaTinggi(ldrValue);
  float nilaiGerakan = keanggotaanGerakan(pirState);

  float nilaiRelayOff = min(nilaiCahayaRendah, nilaiGerakan);
  float nilaiRelayOn = nilaiGerakan;

  // Jika nilai LDR melebihi threshold
  if (ldrValue > ldrThreshold) {
    nilaiRelayOn = 1.0;  // Aktifkan relay sepenuhnya
  }

  // Defuzzifikasi
  float bobotRelayOff = 50.0;
  float bobotRelayOn = 100.0;

  float nilaiRelay = (nilaiRelayOff * bobotRelayOff + nilaiRelayOn * bobotRelayOn) / (nilaiRelayOff + nilaiRelayOn);

  // Kontrol Relay
  if (nilaiRelay > 50.0) {
    digitalWrite(relayPin, LOW);
    Serial.println("Relay ON");
  } else {
     digitalWrite(relayPin, HIGH);
    Serial.println("Relay OFF");
  }

  delay(1000);
}
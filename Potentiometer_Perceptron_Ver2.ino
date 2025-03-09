// Implementation of a Single Layer Perceptron
// Uses two potentiometers as inputs and a pushbutton to trigger training
// Outputs the result on an LED

// VERSION 02

// Pin definitions
const int pot1Pin = A0;
const int pot2Pin = A1;
const int epochButtonPin = 4;
const int outputLed = 5;
const int weight1Led = 7;
const int weight2Led = 8;
const int biasLed = 6;

// Perceptron parameters
float weight1 = 0.0;
float weight2 = 0.0;
float bias = 0.0;
float learningRate = 0.5;

// Dataset
const int datasetSize = 100;
float inputs[datasetSize][2]; // Stores normalized pot values
int targets[datasetSize];     // Stores expected outputs

// Function to normalize potentiometer values to [0, 1]
float normalize(int value) {
  return value / 1023.0;
}

// Activation function (Step function)
int activation(float x) {
  return x >= 0.5 ? 1 : 0;
}

// Perceptron output calculation
int perceptron(float pot1, float pot2) {
  float sum = (pot1 * weight1) + (pot2 * weight2) + bias;
  return activation(sum);
}

// Function to generate dataset
void generateDataset() {
    int positiveSamples = 0;
    int negativeSamples = 0;
    for (int i = 0; i < datasetSize; i++) {
      // Generate random values for pot1 and pot2
      inputs[i][0] = random(1024) / 1023.0; // Normalized pot1
      inputs[i][1] = random(1024) / 1023.0; // Normalized pot2
  
      // Set target based on the fuzzy rule
      if (inputs[i][0] >= 0.1 && inputs[i][0] <= 0.4 && inputs[i][1] >= 0.0 && inputs[i][1] <= 0.7) {
        targets[i] = 1;
        positiveSamples++;
      } else {
        targets[i] = 0;
        negativeSamples++;
      }
    }
  
    // Ensure the dataset is balanced
    int i = 0;
    while (positiveSamples < datasetSize / 2 && i < datasetSize) {
      if (targets[i] == 0) {
        inputs[i][0] = 0.2; // Adjust to fit the positive condition
        inputs[i][1] = 0.5; // Adjust to fit the positive condition
        targets[i] = 1;
        positiveSamples++;
        negativeSamples--;
      }
      i++;
    }
  
    i = 0;
    while (negativeSamples < datasetSize / 2 && i < datasetSize) {
      if (targets[i] == 1) {
        inputs[i][0] = 0.5; // Adjust to fit the negative condition
        inputs[i][1] = 0.8; // Adjust to fit the negative condition
        targets[i] = 0;
        negativeSamples++;
        positiveSamples--;
      }
      i++;
    }
  }

// Function to train the perceptron for one epoch
void trainOneEpoch(int epoch) {
    for (int i = 0; i < datasetSize; i++) {
      // Calculate perceptron output
      int output = perceptron(inputs[i][0], inputs[i][1]);
  
      // Calculate error
      int error = targets[i] - output;
  
      // Store old weights and bias
      float oldWeight1 = weight1;
      float oldWeight2 = weight2;
      float oldBias = bias;
  
      // Update weights and bias
      weight1 += learningRate * error * inputs[i][0];
      weight2 += learningRate * error * inputs[i][1];
      bias += learningRate * error;
  
      // Calculate changes in weights and bias
      float deltaWeight1 = abs(weight1 - oldWeight1);
      float deltaWeight2 = abs(weight2 - oldWeight2);
      float deltaBias = abs(bias - oldBias);
  
      // Map changes to PWM values (0-255)
      int pwmWeight1 = map(deltaWeight1 * 1000, 0, 500, 0, 255);
      int pwmWeight2 = map(deltaWeight2 * 1000, 0, 500, 0, 255);
      int pwmBias = map(deltaBias * 1000, 0, 500, 0, 255);
  
      // Constrain PWM values to be within 0-255
      pwmWeight1 = constrain(pwmWeight1, 0, 255);
      pwmWeight2 = constrain(pwmWeight2, 0, 255);
      pwmBias = constrain(pwmBias, 0, 255);
  
      // Set LED brightness based on changes
      analogWrite(weight1Led, pwmWeight1);
      analogWrite(weight2Led, pwmWeight2);
      analogWrite(biasLed, pwmBias);
  
      // Print weights, bias, and output for this sample
      Serial.print("Epoch: ");
      Serial.print(epoch);
      Serial.print(", Sample: ");
      Serial.print(i);
      Serial.print(", Weight1: ");
      Serial.print(weight1);
      Serial.print(", Weight2: ");
      Serial.print(weight2);
      Serial.print(", Bias: ");
      Serial.print(bias);
      Serial.print(", Output: ");
      Serial.println(output);
      Serial.print(", Target: ");
      Serial.println(targets[i]);
      Serial.print(", Error: ");
      Serial.println(error);
  
      // Small delay to visualize LED changes
      delay(100);
    }
  }
  

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize pins
  pinMode(pot1Pin, INPUT);
  pinMode(pot2Pin, INPUT);
  pinMode(epochButtonPin, INPUT_PULLUP);
  pinMode(outputLed, OUTPUT);
  pinMode(weight1Led, OUTPUT);
  pinMode(weight2Led, OUTPUT);
  pinMode(biasLed, OUTPUT);

  // Generate dataset
  generateDataset();

  // Indicate setup is complete
  Serial.println("Setup complete. Press the epoch button to start training.");
}

void loop() {
  // Read potentiometer values and normalize
  float pot1 = normalize(analogRead(pot1Pin));
  float pot2 = normalize(analogRead(pot2Pin));

  // Get perceptron output
  int output = perceptron(pot1, pot2);

  // Light up output LED based on perceptron output
  digitalWrite(outputLed, output);

  // Print real-time input and output values
  Serial.print("Pot1: ");
  Serial.print(pot1);
  Serial.print(", Pot2: ");
  Serial.print(pot2);
  Serial.print(", Output: ");
  Serial.println(output);

  // Check if epoch button is pressed to train for one epoch
  if (digitalRead(epochButtonPin) == HIGH) {
    static int epoch = 0; // Track the number of epochs
    epoch++;
    Serial.print("Training Epoch: ");
    Serial.println(epoch);

    // Train for one epoch
    trainOneEpoch(epoch);

    // Debounce delay
    delay(500);
  }
}
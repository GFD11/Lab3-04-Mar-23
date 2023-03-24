#include "mbed.h"
#include "C12832.h"
#include "LM75B.h"

C12832 lcd(p5, p7, p6, p8, p11);
LM75B temp_sensor(p28,p27);

// Create the local filesystem 
LocalFileSystem local("local");

const int DATA_SIZE = 100;
const int LOG_INTERVAL_SEC = 3; // log data every 5 minutes
const char* FILENAME = "/local/data.txt";

float data[DATA_SIZE];
int data_count = 0;

float get_min(float *data, int size) {
  float min = data[0];
  for (int i = 1; i < size; i++) {
    if (data[i] < min) {
      min = data[i];
    }
  }
  return min;
}

float get_max(float *data, int size) {
  float max = data[0];
  for (int i = 1; i < size; i++) {
    if (data[i] > max) {
      max = data[i];
    }
  }
  return max;
}

float get_average(float *data, int size) {
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += data[i];
  }
  return sum / size;
}

int main() {  
    Timer log_timer;
    log_timer.start(); 

    while (1) {
        // check if it's time to log data
        if (log_timer.read() > LOG_INTERVAL_SEC) {
            log_timer.reset();

            //Temp measured
            float temp = temp_sensor.read();
            data[data_count] = temp;
            data_count++;

           //Storage of data
           FILE* fp = fopen(FILENAME, "a");
            if (fp != NULL) {
                fprintf(fp, "%.2f\n", temp);
                fclose(fp);
            }
        }

        //Files read from storage
        FILE* fp = fopen(FILENAME, "r");
        if (fp != NULL) {
            int i = 0;
            while (!feof(fp) && i < DATA_SIZE) {
                fscanf(fp, "%f", &data[i]);
                i++;
            }
            fclose(fp);

            float min = get_min(data, data_count);
            float max = get_max(data, data_count);
            float avg = get_average(data, data_count);
            

            // show temp data on LCD
            lcd.cls();
            lcd.locate(0,3);
            lcd.printf("Min temp: %.2fC\n", min);
            lcd.printf("Max temp: %.2fC\n", max);
            lcd.printf("Avg temp: %.2fC\n", avg);
        }
        
      
    }
}

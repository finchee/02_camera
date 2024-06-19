#include <Arduino.h>
#include "esp_camera.h"

// put define here:
#define portTICK_RATE_MS portTICK_PERIOD_MS

#define CAM_PIN_PWDN -1  //power down is not used
#define CAM_PIN_RESET -1 //software reset will be performed
#define CAM_PIN_XCLK 39
#define CAM_PIN_SIOD 21
#define CAM_PIN_SIOC 1

#define CAM_PIN_D7 40
#define CAM_PIN_D6 38
#define CAM_PIN_D5 10
#define CAM_PIN_D4 12
#define CAM_PIN_D3 07
#define CAM_PIN_D2 48
#define CAM_PIN_D1 47
#define CAM_PIN_D0 6
#define CAM_PIN_VSYNC 42
#define CAM_PIN_HREF 41
#define CAM_PIN_PCLK 11

static const char *TAG = "example:take_picture";


// put function declarations here:
static camera_config_t camera_config = {
    .pin_pwdn = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sccb_sda = CAM_PIN_SIOD,
    .pin_sccb_scl = CAM_PIN_SIOC,

    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_JPEG, //YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = FRAMESIZE_QVGA,    //QQVGA-UXGA, For ESP32, do not use sizes above QVGA when not JPEG. The performance of the ESP32-S series has improved a lot, but JPEG mode always gives better frame rates.

    .jpeg_quality = 12, //0-63, for OV series camera sensors, lower number means higher quality
    .fb_count = 1,       //When jpeg mode is used, if fb_count more than one, the driver will work in continuous mode.
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
};

static esp_err_t init_camera(void)
{
    //initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Camera Init Failed");
        return err;
    }

    return ESP_OK;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // 初始化串口通信
  Serial.setDebugOutput(true);
  Serial.println();
  if(ESP_OK != init_camera()) {
      ESP_LOGE(TAG, "Camera Init Failed");;
  }

}

void loop() {
  // put your main code here, to run repeatedly:
    // Serial.print("Taking picture...\r\n");
    camera_fb_t *pic = esp_camera_fb_get();
    char data[10];
    String msg;
    for (int i = 0; i < pic->len; i++)
    {
        sprintf(data, "%02X", *((pic->buf + i)));
        msg += data;                     
    }
    if (msg.length()>0)
    {                                    
        Serial.print(msg);//直接打印十六进制的字符串,客户端不能直接解码
    }
    esp_camera_fb_return(pic);
    vTaskDelay(1000 / portTICK_RATE_MS);

}


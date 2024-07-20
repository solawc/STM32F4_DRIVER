#include "spi.h"

struct spi_data {
    
    SPI_HandleTypeDef *handle;

    /* UART 对应的GPIO */
    GPIO_TypeDef *spi_gpio_port;
    uint16_t spi_sck;
    uint16_t spi_miso;
    uint16_t spi_mosi;
    uint16_t spi_cs;
};

struct spi_device {
    char *name;
    struct spi_data *data;
    int (*spi_dev_init)(struct spi_device *spi);
    int (*spi_dev_read_write_data)(struct spi_device *spi, uint8_t wdate, uint8_t *rdata);
    int (*spi_dev_cs_set)(struct spi_device *spi, bool state);   
    int (*spi_dev_set_speed)(struct spi_device *spi, uint32_t speed);
};

int spi_dev_init(struct spi_device *spi) {

    GPIO_InitTypeDef GPIO_Init;

    GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init.Pin = spi->data->spi_cs;
    GPIO_Init.Pull = GPIO_PULLUP;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(spi->data->spi_gpio_port, &GPIO_Init);

    GPIO_Init.Alternate = GPIO_AF5_SPI5;
    GPIO_Init.Mode = GPIO_MODE_AF_PP;
    GPIO_Init.Pin = spi->data->spi_sck  |   \
                    spi->data->spi_miso |   \
                    spi->data->spi_mosi;
    GPIO_Init.Pull = GPIO_PULLUP;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(spi->data->spi_gpio_port, &GPIO_Init);

    HAL_SPI_Init(spi->data->handle);
}

int spi_dev_read_write_data(struct spi_device *spi, uint8_t wdate, uint8_t *rdata) {

    HAL_StatusTypeDef staus = HAL_OK;

    staus = HAL_SPI_TransmitReceive(spi->data->handle, &wdate, rdata, 1, 1000);

    if(staus != HAL_OK)
        return -1;
    else 
        return 0;
}

int spi_dev_cs_set(struct spi_device *spi, bool state) {

    if(state) {
        HAL_GPIO_WritePin(spi->data->spi_gpio_port, spi->data->spi_cs, GPIO_PIN_SET);
    }else {
        HAL_GPIO_WritePin(spi->data->spi_gpio_port, spi->data->spi_cs, GPIO_PIN_RESET);
    }

    return 0;
}

SPI_HandleTypeDef flash_spi = {

    .Instance = SPI5,
    .Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8,
    .Init.CLKPhase          = SPI_PHASE_2EDGE,
    .Init.CLKPolarity       = SPI_POLARITY_HIGH,
    .Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE,
    .Init.CRCPolynomial     = 7,
    .Init.DataSize          = SPI_DATASIZE_8BIT,
    .Init.Direction         = SPI_DIRECTION_2LINES,
    .Init.FirstBit          = SPI_FIRSTBIT_MSB,
    .Init.Mode              = SPI_MODE_MASTER,
    .Init.NSS               = SPI_NSS_SOFT,
    .Init.TIMode            = SPI_TIMODE_DISABLE,
};

struct spi_data flash_spi_data = {
    
    &flash_spi,
    GPIOF,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_6,
};
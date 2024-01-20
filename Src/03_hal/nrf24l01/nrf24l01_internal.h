#ifndef NRF24L01_INTERNAL_H
#define NRF24L01_INTERNAL_H

// REGISTERS
// CONFIG
#define NRF_REG_CONFIG                  0x00
#define NRF_MASK_RX_DR      0x6
#define NRF_MASK_TX_DS      0x5
#define NRF_MASK_MAX_RT     0x4
#define NRF_EN_CRC          0x3
#define NRF_CRCO            0x2
#define NRF_PWR_UP          0x1
#define NRF_PRIM_RX         0x0

// AUTO ACK 
#define NRF_REG_EN_AA                   0x01
#define NRF_ENAA_P5         0x5
#define NRF_ENAA_P4         0x4
#define NRF_ENAA_P3         0x3
#define NRF_ENAA_P2         0x2
#define NRF_ENAA_P1         0x1
#define NRF_ENAA_P0         0x0

// ENABLE RX PIPE
#define NRF_REG_EN_RXADDR               0x02
#define NRF_ERX_P5          0x5
#define NRF_ERX_P4          0x4
#define NRF_ERX_P3          0x3
#define NRF_ERX_P2          0x2
#define NRF_ERX_P1          0x1
#define NRF_ERX_P0          0x0

// SETUP ADDR WIDTHS
#define NRF_REG_SETUP_AW                0x03
#define NRF_AW              0x0

// SETUP AUTO RETRANSMISSION
#define NRF_REG_SETUP_RETR              0x04
#define NRF_ARD             0x4
#define NRF_ARC             0x0

// RF CHANNEL
#define NRF_REG_RF_CH                   0x05
#define NRF_RF_CH           0x0

// RF SETUP
#define NRF_REG_RF_SETUP                0x06
#define NRF_CONT_WAVE       0x7
#define NRF_RF_DR_LOW       0x5
#define NRF_PLL_LOCK        0x4
#define NRF_RF_DR_HIGH      0x3
#define NRF_RF_PWR          0x1

// STATUS  shifted out whenever sending command 
#define NRF_REG_STATUS                  0x06
#define NRF_RX_DR           0x6
#define NRF_TX_DS           0x5
#define NRF_MAX_RT          0x4
#define NRF_RX_P_NO         0x1
#define NRF_TX_FULL         0x0

// TRANSMIT OBSERVE
#define NRF_REG_OBSERVE_TX              0x08
#define NRF_PLOS_CNT        0x4
#define NRF_ARC_CNT         0x0

// RECEIVED POWER DETECTOR
#define NRF_REG_RPD                     0x09
#define NRF_RPD             0x0

// RECEIVE ADDRESS for data pipe Px [1-5 bytes]
#define NRF_REG_RX_ADDR_P0              0x0A
#define NRF_REG_RX_ADDR_P1              0x0B
// RECEIVE ADDRESS for data pipe Px [1 byte rest equal to P1]
#define NRF_REG_RX_ADDR_P2              0x0C
#define NRF_REG_RX_ADDR_P3              0x0D
#define NRF_REG_RX_ADDR_P4              0x0E
#define NRF_REG_RX_ADDR_P5              0x0F

// TX ADDER
#define NRF_REG_TX_ADDR                 0x10

// PAYLOAD SIZE for pipe Px
#define NRF_REG_PW_P0                   0x11
#define NRF_REG_PW_P1                   0x12
#define NRF_REG_PW_P2                   0x13
#define NRF_REG_PW_P3                   0x14
#define NRF_REG_PW_P4                   0x15
#define NRF_REG_PW_P5                   0x16

// FIFO STATUS
#define NRF_REG_FIFO_STATUS             0x17
#define NRF_TX_REUSE        0x6
#define NRF_TX_FULL         0x5
#define NRF_TX_EMPTY        0x4
#define NRF_RX_FULL         0x1
#define NRF_RX_EMPTY        0x0

// DYNAMIC PAYLOAD LENGTH
#define NRF_REG_DYNPD                   0x1C
#define NRF_DPL_P5          0x5
#define NRF_DPL_P4          0x4
#define NRF_DPL_P3          0x3
#define NRF_DPL_P2          0x2
#define NRF_DPL_P1          0x1
#define NRF_DPL_P0          0x0

// FEATURE REGISTER
#define NRF_REG_FEATURE                 0x1D
#define NRF_EN_DPL          0x2
#define NRF_EN_ACK_PAY      0x1
#define NRF_EN_DYN_ACK      0x0


// COMMANDS
#define NRF_COMM_R_REGISTER             0x00    // OR with REGISTER
#define NRF_COMM_W_REGISTER             0x20    // OR with REGISTER
#define NRF_COMM_R_RX_PAYLOAD           0x61
#define NRF_COMM_W_TX_PAYLOAD           0xA0
#define NRF_COMM_FLUSH_TX               0xE1
#define NRF_COMM_FLUSH_RX               0xE2
#define NRF_COMM_REUSE_TX_PL            0xE3
#define NRF_COMM_r_RX_PL_WID            0x60
#define NRF_COMM_W_ACK_PAYLOAD          0xA8    // OR with PIPE
#define NRF_COMM_W_TX_PAYLOAD_NO_ACK    0xB0
#define NRF_COMM_NOP                    0xFF

// COMMANDS max data size 
#define NRF_COMM_R_REGISTER_MAX_DATA             5
#define NRF_COMM_W_REGISTER_MAX_DATA             5
#define NRF_COMM_R_RX_PAYLOAD_MAX_DATA           32
#define NRF_COMM_W_TX_PAYLOAD_MAX_DATA           32
#define NRF_COMM_R_RX_PL_WID_MAX_DATA            1
#define NRF_COMM_W_ACK_PAYLOAD_MAX_DATA          32
#define NRF_COMM_W_TX_PAYLOAD_NO_ACK_MAX_DATA    32


#endif /* NRF24L01_INTERNAL_H */

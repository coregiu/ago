/**
  ******************************************************************************
  *
  * Implement of vehicle executor.
  * author: coregiu
  *
  *
  ******************************************************************************
**/
#include <vehicle_executor.h>

// the position of gpio in CAR_STATE_LIST array.
enum gpio_position
{
    LEFT_FRONT_1_POSITION   = 0,
    LEFT_FRONT_2_POSITION   = 1,
    LEFT_BACK_1_POSITION    = 2,
    LEFT_BACK_2_POSITION    = 3,
    RIGHT_FRONT_1_POSITION  = 4,
    RIGHT_FRONT_2_POSITION  = 5,
    RIGHT_BACK_1_POSITION   = 6,
    RIGHT_BACK_2_POSITION   = 7
};

// car sate to set to gpio: {LEFT_EN, LEFT_MV, LEFT_BK, RIGHT_EN, RIGHT_MV, RIGHT_BK}
const char VEHICLE_STATE_LIST[12][8] = {{0, 0, 0, 0, 0, 0, 0, 0},  // 0, init/stop
                                        {1, 0, 1, 0, 1, 0, 1, 0},  // 1, move, all move
                                        {0, 1, 0, 1, 0, 1, 0, 1},  // 2, back, all back
                                        {0, 1, 1, 0, 1, 0, 0, 1},  // 3, left
                                        {1, 0, 0, 1, 0, 1, 1, 0},  // 4, right
                                        {0, 0, 1, 0, 1, 0, 0, 0},  // 5, left_front
                                        {1, 0, 0, 0, 0, 0, 1, 0},  // 6, right_front
                                        {0, 1, 0, 0, 0, 0, 0, 1},  // 7, left_back
                                        {0, 0, 0, 1, 0, 1, 0, 0},  // 8, right_back
                                        {0, 0, 0, 0, 1, 0, 1, 0},  // 9, left_turn
                                        {1, 0, 1, 0, 0, 0, 0, 0},  // 10, right_turn
                                        {1, 0, 1, 0, 0, 1, 0, 1}   // 11, turn over
                                        };

enum vehicle_state current_car_status = STOP;

void exec_vehicle_state_update(enum vehicle_state run_state)
{
    if (current_car_status != run_state)
    {
        IN1  = VEHICLE_STATE_LIST[run_state][LEFT_FRONT_1_POSITION];
        IN2  = VEHICLE_STATE_LIST[run_state][LEFT_FRONT_2_POSITION];
        IN3  = VEHICLE_STATE_LIST[run_state][LEFT_BACK_1_POSITION];
        IN4  = VEHICLE_STATE_LIST[run_state][LEFT_BACK_2_POSITION];
        IN5  = VEHICLE_STATE_LIST[run_state][RIGHT_FRONT_1_POSITION];
        IN6  = VEHICLE_STATE_LIST[run_state][RIGHT_FRONT_2_POSITION];
        IN7  = VEHICLE_STATE_LIST[run_state][RIGHT_BACK_1_POSITION];
        IN8  = VEHICLE_STATE_LIST[run_state][RIGHT_BACK_2_POSITION];
        current_car_status = run_state;
    }
}

void init_vehicle_state()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); //使能PA,PB, PC端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_IN1;           //GPIO PA11 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);            //根据设定参数初始化GPIOA.IN1
    IN1 = 0;

    GPIO_InitStructure.GPIO_Pin = GPIO_IN2;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    IN2 = 0;

    GPIO_InitStructure.GPIO_Pin = GPIO_IN3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    IN3 = 0;

    GPIO_InitStructure.GPIO_Pin = GPIO_IN4;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    IN4 = 0;

    GPIO_InitStructure.GPIO_Pin = GPIO_IN5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    IN5 = 0;

    GPIO_InitStructure.GPIO_Pin = GPIO_IN6;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    IN6 = 0;

    GPIO_InitStructure.GPIO_Pin = GPIO_IN7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    IN7 = 0;

    GPIO_InitStructure.GPIO_Pin = GPIO_IN8;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    IN8 = 0;
}

void update_vehicle_state(char command)
{
    switch (command)
        {
        case COMMAND_STOP:
            // uart_log_string_data("e:1"); // send 1
            exec_vehicle_state_update(STOP);
            break;
        case COMMAND_RUN:
            // uart_log_string_data("e:2"); // send 2
            exec_vehicle_state_update(MOVE);
            break;
        case COMMAND_BACK:
            // uart_log_string_data("e:3"); // send 3
            exec_vehicle_state_update(BACK);
            break;
        case COMMAND_LEFT_RUN:
            // uart_log_string_data("e:4"); // send 4
            exec_vehicle_state_update(LEFT);
            break;
        case COMMAND_RIGHT_RUN:
            // uart_log_string_data("e:4"); // send 4
            exec_vehicle_state_update(RIGHT);
            break;
        case COMMAND_LEFT_FRONT:
            // uart_log_string_data("e:1"); // send 5
            exec_vehicle_state_update(LEFT_FRONT);
            break;
        case COMMAND_RIGHT_FRONT:
            // uart_log_string_data("e:2"); // send 6
            exec_vehicle_state_update(RIGHT_FRONT);
            break;
        case COMMAND_LEFT_BACK:
            // uart_log_string_data("e:3"); // send 7
            exec_vehicle_state_update(LEFT_BACK);
            break;
        case COMMAND_RIGHT_BACK:
            // uart_log_string_data("e:4"); // send 8
            exec_vehicle_state_update(RIGHT_BACK);
            break;
        case COMMAND_LEFT_TURN:
            exec_vehicle_state_update(LEFT_TURN);
            // stop();
            break;
        case COMMAND_RIGHT_TURN:
            // uart_log_string_data("e:2"); // send A
            exec_vehicle_state_update(RIGHT_TURN);
            break;
        case COMMAND_GO_BACK:
            // uart_log_string_data("e:3"); // send B
            // exec_vehicle_state_update(LEFT);
            break;
        case COMMAND_TURN_OUT:
            // uart_log_string_data("e:4"); // send C
            exec_vehicle_state_update(TURN_OVER);
            break;
        case COMMAND_TEST_VEHICLE:
            // uart_log_string_data("e:4"); // send D
            break;
        default:
            break;
        }
}

const struct module_command_executor vehicle_executor = {init_vehicle_state, update_vehicle_state};

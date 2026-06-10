# io_stm_nucleo



## Src
### Examples
#### LED와 버튼
- example 1: LED On/Off
- example 2: 버튼 On 시 LED On, 버튼 Off 시 LED off
- example 3: 버튼 On 시 LED 순차 점멸
- example 4: 버튼 On 시 LED 순차 점멸방향 전환
- example 5: 버튼 On 시 LED 깜빡임 1초에 5회 / 버튼 Off 시 LED 깜빡임 1초에 1회
- example 6: 버튼 입력 시 LED 방향 전환 - polling
- example 7: 버튼 입력 시 LED 방향 전환 - interrupt
- example 8: 버튼 3초간 입력시 LED 10초간 켜졌다 꺼짐
- example 9: 버튼 1초 누르면 노란색 LED만 1초간 켜짐, 버튼 3초간 누르면 파랑색 LED만 1초간 켜짐, 버튼 5초간 누르면 빨간색 LED만 1초간 켜짐 
- example 10: 버튼 1회 누르면 노란색 LED만 1초간 켜짐, 버튼 2회 누르면 파랑색 LED만 1초간 켜짐, 버튼 3회 누르면 빨간색 LED만 1초간 켜짐

### 심화 GPIO
 - example 1 : L9110팬 모듈 양방향 회전
 - example 2 : 9g 서보 양방향 회전
   - 1번 꾹 누르면 정방향 회전
   - 2번 꾹 누르면 역방향 회전
 - example 3 : 5v 스테핑모터 양방향 회전
 - example 4 : 온도센서 와 모터
    - 25도 이하면 시계방향, 초과면 반시계방향
 - example 5 : 온습도 센서와 모터
    - 습도 높아지면 모터 회전
 - example 6 : 터치패드 입력 하면 7-segment에 숫자 나타나도록
 - example 7 : 버튼 클릭 시 7-segment 계수기 처럼 증가
 
#### 지렁이 게임
 - example 1 : LED 매트릭스에 GMOV 그리기
 - example 2 :LED 매트릭스 1,1 ~16,8 중 하나의 LED를 켜고 그 좌표를 7-segment에 출력
 - example 3 : console에서 좌표 입력 하여 LED 매트릭스 특정 좌표에 값 표출
 - example 4 : LED 매트릭스에서 이동하는 지렁이 그리기
    - 0.5 초에 1칸씩 이동
 - example 5 : 지렁이 머리를 중심으로 지렁이 이동 좌표를 console에 표시
 - example 6 : 지렁이 머리를  중심으로 좌표를 7-segment에 표줄
 - example 7 : 버튼에 따라 지렁이 방향 바꾸기
    - 1번 누르면 왼쪽
    - 2번 누르면 오른쪽
 - example 8 : 커지는 지렁이 만들기 
    - 랜덤하게 점을 뿌리고 머리가 점을 만나면 지렁이 길이 + 1
    - 10초 이상 아무것도 안먹으면 죽음
 - example 9 : 온도 센서
    - 버튼 입력은 무시하고 온도 센서 30도 이상 감지 시 방향 변화
    - 온도 7-segment에 표출
 - example 10 : 모터 구동
    - LED 매트릭스에서 지렁이 움직임 감지
    - LED 값의 변화에 따라 지렁이가 방향을 전환하면 서보모터 30도 씩 변환


#### DAC
#### ADC
#### Pulse modulation
##### PWM
##### PFM
##### Combination
#### Communication
##### UART
##### SPI
##### I2C


#### Signal processing

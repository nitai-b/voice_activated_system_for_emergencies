; external declaration of sound table:

    .EXTRN (const) SNDTBL_AUDIOVASE



; external declarations of addresses of sounds for direct access:

    .EXTRN (const) SX_zero
    .EXTRN (const) SX_one
    .EXTRN (const) SX_two
    .EXTRN (const) SX_three
    .EXTRN (const) SX_four
    .EXTRN (const) SX_five
    .EXTRN (const) SX_six
    .EXTRN (const) SX_seven
    .EXTRN (const) SX_eight
    .EXTRN (const) SX_nine
    .EXTRN (const) SX_hello_please_say_a_command
    .EXTRN (const) SX_please_say_a_command
    .EXTRN (const) SX_command_accepted
    .EXTRN (const) SX_who_would_you_like_to_call
    .EXTRN (const) SX_calling
    .EXTRN (const) SX_say_answer_to_answer_the_call
    .EXTRN (const) SX_say_reject_to_reject_the_call
    .EXTRN (const) SX_say_send_to_place_the_call
    .EXTRN (const) SX_you_have_an_incoming_call
    .EXTRN (const) SX_turning_off_led
    .EXTRN (const) SX_turning_on_LED
    .EXTRN (const) SX_turning_off_light_bulb
    .EXTRN (const) SX_turning_on_light_bulb
    .EXTRN (const) SX_is_trying_to_reach_you
    .EXTRN (const) SX_doctor


; equates defining sounds as entries in the sound table:

SND_zero                .EQU 1
SND_one                 .EQU 2
SND_two                 .EQU 3
SND_three               .EQU 4
SND_four                .EQU 5
SND_five                .EQU 6
SND_six                 .EQU 7
SND_seven               .EQU 8
SND_eight               .EQU 9
SND_nine                .EQU 10
SND_hello_please_say_a_command .EQU 11
SND_please_say_a_command .EQU 12
SND_command_accepted    .EQU 13
SND_who_would_you_like_to_call .EQU 14
SND_calling             .EQU 15
SND_say_answer_to_answer_the_call .EQU 16
SND_say_reject_to_reject_the_call .EQU 17
SND_say_send_to_place_the_call .EQU 18
SND_you_have_an_incoming_call .EQU 19
SND_turning_off_led     .EQU 20
SND_turning_on_LED      .EQU 21
SND_turning_off_light_bulb .EQU 22
SND_turning_on_light_bulb .EQU 23
SND_is_trying_to_reach_you .EQU 24
SND_doctor              .EQU 25


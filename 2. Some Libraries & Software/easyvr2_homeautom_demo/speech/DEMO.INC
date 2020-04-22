; external declaration of sound table:

    .EXTRN (const) SNDTBL_DEMO



; external declarations of addresses of sounds for direct access:

    .EXTRN (const) SX_Access_denied
    .EXTRN (const) SX_Access_granted
    .EXTRN (const) SX_Hello
    .EXTRN (const) SX_Please_repeat
    .EXTRN (const) SX_Please_say_your_password
    .EXTRN (const) SX_Hello_give_command
    .EXTRN (const) SX_Please_say_name
    .EXTRN (const) SX_Pwd_activated


; equates defining sounds as entries in the sound table:

SND_Access_denied       .EQU 1
SND_Access_granted      .EQU 2
SND_Hello               .EQU 3
SND_Please_repeat       .EQU 4
SND_Please_say_your_password .EQU 5
SND_Hello_give_command  .EQU 6
SND_Please_say_name     .EQU 7
SND_Pwd_activated       .EQU 8


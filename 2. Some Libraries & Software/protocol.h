#ifndef PROTOCOL_H
#define PROTOCOL_H

#define null            '!' //to initialise the received variable
#define CMD_BREAK       'b' // abort recog or ping
#define CMD_SLEEP       's' // go to power down
#define CMD_KNOB        'k' // set si knob <1>
#define CMD_MIC_DIST    'k' // set microphone (<1>=-1) distance <2>
#define CMD_LEVEL       'v' // set sd level <1>
#define CMD_VERIFY_RP   'v' // verify filesystem (<1>=-1) with flags <2> (0=check only, 1=fix)
#define CMD_LANGUAGE    'l' // set si language <1>
#define CMD_LIPSYNC     'l' // start real-time lipsync (<1>=-1) with threshold <2-3>, timeout <4-5>
#define CMD_TIMEOUT     'o' // set timeout <1>
#define CMD_RECOG_SI    'i' // do si recog from ws <1>
#define CMD_TRAIN_SD    't' // train sd command at group <1> pos <2>
#define CMD_TRAILING    't' // set trailing (<1>=-1) silence <2> (0-31 = 100-875 milliseconds)
#define CMD_GROUP_SD    'g' // insert new command at group <1> pos <2>
#define CMD_UNGROUP_SD  'u' // remove command at group <1> pos <2>
#define CMD_RECOG_SD    'd' // do sd recog at group <1> (0 = trigger mixed si/sd)
#define CMD_DUMP_RP     'd' // dump message (<1>=-1) at pos <2>
#define CMD_ERASE_SD    'e' // reset command at group <1> pos <2>
#define CMD_ERASE_RP    'e' // erase recording (<1>=-1) at pos <2>
#define CMD_NAME_SD     'n' // label command at group <1> pos <2> with length <3> name <4-n>
#define CMD_COUNT_SD    'c' // get command count for group <1>
#define CMD_DUMP_SD     'p' // read command data at group <1> pos <2>
#define CMD_PLAY_RP     'p' // play recording (<1>=-1) at pos <2> with flags <3>
#define CMD_MASK_SD     'm' // get active group mask
#define CMD_RESETALL    'r' // reset all memory (commands/groups and messages), with <1>='R'
#define CMD_RESET_SD    'r' // reset only commands/groups, with <1>='D'
#define CMD_RESET_RP    'r' // reset only messages, with <1>='M'
#define CMD_RECORD_RP   'r' // record message (<1>=-1) at pos <2> with bits <3> and timeout <4>
#define CMD_ID          'x' // get version id
#define CMD_DELAY       'y' // set transmit delay <1> (log scale)
#define CMD_BAUDRATE    'a' // set baudrate <1> (bit time, 1=>115200)
#define CMD_QUERY_IO    'q' // configure, read or write I/O pin <1> of type <2>
#define CMD_PLAY_SX     'w' // wave table entry <1-2> (10-bit) playback at volume <3>
#define CMD_PLAY_DTMF   'w' // play (<1>=-1) dial tone <2> for duration <3>
#define CMD_DUMP_SX     'h' // dump wave table entries
#define CMD_DUMP_SI     'z' // dump si settings for ws <1> (or total ws count if -1)
#define CMD_SEND_SN     'j' // send sonicnet token with bits <1> index <2-3> at time <4-5>
#define CMD_RECV_SN     'f' // receive sonicnet token with bits <1> rejection <2> timeout <3-4>
#define CMD_FAST_SD     'f' // set sd/sv (<1>=-1) to use fast recognition <2> (0=normal/default, 1=fast)

#define CMD_SERVICE     '~' // send service request
#define SVC_EXPORT_SD   'X' // request export of command <2> in group <1> as raw dump
#define SVC_IMPORT_SD   'I' // request import of command <2> in group <1> as raw dump
#define SVC_VERIFY_SD   'V' // verify training of imported raw command <2> in group <1>

#define STS_SERVICE     '~' // get service reply
#define SVC_DUMP_SD     'D' // provide raw command data <1-512> followed by checksum <513-516>

#define STS_MASK        'k' // mask of active groups <1-8>
#define STS_COUNT       'c' // count of commands <1> (or number of ws <1>)
#define STS_AWAKEN      'w' // back from power down mode
#define STS_DATA        'd' // provide training <1>, conflict <2>, command label <3-35> (counted string)
#define STS_ERROR       'e' // signal error code <1-2>
#define STS_INVALID     'v' // invalid command or argument
#define STS_TIMEOUT     't' // timeout expired
#define STS_LIPSYNC     'l' // lipsync stream follows
#define STS_INTERR      'i' // back from aborted recognition (see 'break')
#define STS_SUCCESS     'o' // no errors status
#define STS_RESULT      'r' // recognised sd command <1> - training similar to sd <1>
#define STS_SIMILAR     's' // recognised si <1> (in mixed si/sd) - training similar to si <1>
#define STS_OUT_OF_MEM  'm' // no more available commands (see 'group')
#define STS_ID          'x' // provide version id <1>
#define STS_PIN         'p' // return pin state <1>
#define STS_TABLE_SX    'h' // table entries count <1-2> (10-bit), table name <3-35> (counted string)
#define STS_GRAMMAR     'z' // si grammar: flags <1>, word count <2>, labels... <3-35> (n counted strings)
#define STS_TOKEN       'f' // received sonicnet token <1-2>
#define STS_MESSAGE     'g' // message status <1> (0=empty, 4/8=bits format), length <2-7>

// protocol arguments are in the range 0x40 (-1) to 0x60 (+31) inclusive
#define ARG_MIN           0x40   //argument value (-1) // ASCII (@)
#define ARG_ZERO          0x41   //argument value (0)  // ASCII (A)
#define ARG_ONE           0x42   //argument value (1)  // ASCII (B)
#define ARG_TWO           0x43   //argument value (2)  // ASCII (C)
#define ARG_THREE         0x44   //argument value (3)  // ASCII (D)
#define ARG_FOUR          0x45   //argument value (4)  // ASCII (E)
#define ARG_FIVE          0x46   //argument value (5)  // ASCII (F)
#define ARG_SIX           0x47   //argument value (6)  // ASCII (G)
#define ARG_SEVEN         0x48   //argument value (7)  // ASCII (H)
#define ARG_EIGHT         0x49   //argument value (8)  // ASCII (I)
#define ARG_NINE          0x4A   //argument value (9)  // ASCII (J)
#define ARG_TEN           0x4B   //argument value (10) // ASCII (K)
#define ARG_ELEVEN        0x4C   //argument value (11) // ASCII (L)
#define ARG_TWELVE        0x4D   //argument value (12) // ASCII (M)
#define ARG_THIRTEEN      0x4E   //argument value (13) // ASCII (N)
#define ARG_FOURTEEN      0x4F   //argument value (14) // ASCII (O)
#define ARG_FIFTEEN       0x50   //argument value (15) // ASCII (P)
#define ARG_SIXTEEN       0x51   //argument value (16) // ASCII (Q)
#define ARG_SEVENTEEN     0x52   //argument value (17) // ASCII (R)
#define ARG_EIGHTEEN      0x53   //argument value (18) // ASCII (S)
#define ARG_NINETEEN      0x54   //argument value (19) // ASCII (T)
#define ARG_TWENTY        0x55   //argument value (20) // ASCII (U)
#define ARG_TWENTYONE     0x56   //argument value (21) // ASCII (V)
#define ARG_TWENTYTWO     0x57   //argument value (22) // ASCII (W)
#define ARG_TWENTYTHREE   0x58   //argument value (23) // ASCII (X)
#define ARG_TWENTYFOUR    0x59   //argument value (24) // ASCII (Y)
#define ARG_TWENTYFIVE    0x5A   //argument value (25) // ASCII (Z)
#define ARG_TWENTYSIX     0x5B   //argument value (26) // ASCII (^)
#define ARG_TWENTYSEVEN   0x5C   //argument value (27) // ASCII ([)
#define ARG_TWENTYEIGHT   0x5D   //argument value (28) // ASCII (\)
#define ARG_TWENTYNINE    0x5E   //argument value (29) // ASCII (])
#define ARG_THIRTY        0x5F   //argument value (30) // ASCII (_)
#define ARG_MAX           0x60   //argument value (31) // ASCII (`)
#define ARG_ACK           0x20   // to read more status arguments

#endif //PROTOCOL_H
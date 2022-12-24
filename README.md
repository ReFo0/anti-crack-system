# anti crack system

I think that this system that I have coded can be improved further :)

* Added antiattach  23.12.2022

* Added self-remapping code 23.12.2022

                            self-remapping code
                                   
                                        before
         0000000140001000 000000000000F000 Reserved (0000000140000000)   IMG             ERWC-
         0000000140010000 0000000000002000  ".text"                      IMG    ER---    ERWC-
         0000000140012000 000000000000E000 Reserved (0000000140000000)   IMG             ERWC-
         0000000140020000 0000000000002000  ".rdata"                     IMG    -R---    ERWC-
         0000000140022000 000000000000E000 Reserved (0000000140000000)   IMG             ERWC-
         0000000140030000 0000000000001000  ".data"                      IMG    -RW--    ERWC-
         0000000140031000 000000000000F000 Reserved (0000000140000000)   IMG             ERWC-
         0000000140040000 0000000000001000  ".pdata"                     IMG    -R---    ERWC-
         0000000140041000 000000000000F000 Reserved (0000000140000000)   IMG             ERWC-
         0000000140050000 0000000000001000  ".rsrc"                      IMG    -R---    ERWC-
         0000000140051000 000000000000F000 Reserved (0000000140000000)   IMG             ERWC-

                                          after
          0000000140000000 0000000000001000                               MAP    -R---    -R---
          0000000140010000 0000000000002000                               MAP    ER---    ER---
          0000000140020000 0000000000002000                               MAP    -R---    -R---
          0000000140030000 0000000000001000                               MAP    -RW--    -RW--
          0000000140040000 0000000000001000                               MAP    -R---    -R---
          0000000140050000 0000000000001000                               MAP    -R---    -R---

; SYNTAX TEST "G-code.sublime-syntax"

; This is a G-code comment
;^comment

G1 X100 Y100 ; Move to 100,100
;^entity.command.gcode
;  ^keyword.param.gcode
;   ^constant.numeric.param.gcode
;              ^comment

T0 (This is a comment) S12
;^entity.command.gcode
;  ^punctuation.paren.comment.open
;   ^paren.comment.gcode
;                    ^punctuation.paren.comment.close
;                      ^keyword.param.gcode
;                       ^constant.numeric.param.gcode

M20 P'/path/to/macro/macro.g' R12
;<-entity.command.gcode
;^constant.numeric.command.gcode
;   ^keyword.param.gcode
;    ^punctuation.quote.single.open.gcode
;       ^string.quoted.single.gcode
;                           ^punctuation.quote.single.close.gcode
;                             ^keyword.param.gcode
;                              ^constant.numeric.param.gcode

M117 This is a message   ; and comment
;<-entity.command.gcode
;^constant.numeric.command.gcode
;    ^string.unquoted.gcode
;                        ^punctuation.comment.eol.start
;                          ^comment.gcode

M118 This is a message   ; and comment
;<-entity.command.gcode
;^constant.numeric.command.gcode
;    ^string.unquoted.gcode
;                        ^punctuation.comment.eol.start
;                          ^comment.gcode

M98 P'/path/to/macro/macro.g' R12
;<-entity.command.gcode
;^constant.numeric.command.gcode
;   ^keyword.param.gcode
;    ^punctuation.quote.single.open.gcode
;       ^string.quoted.single.gcode
;                           ^punctuation.quote.single.close.gcode

M98 P"/path/to/macro/macro.g" R12
;<-entity.command.gcode
;^constant.numeric.command.gcode
;   ^keyword.param.gcode
;    ^punctuation.quote.double.open.gcode
;       ^string.quoted.double.gcode
;                           ^punctuation.quote.double.close.gcode

M32 S100 P0 !/path/file.gco#
;<-entity.command.gcode
;^constant.numeric.command.gcode
;   ^keyword.param.gcode
;    ^constant.numeric.param.gcode
;           ^punctuation.string.path.open.gcode
;                ^string.unquoted.path.gcode
;                          ^punctuation.string.path.close.gcode

G28 ; Home All
;<-entity.command.gcode
;^constant.numeric.command.gcode
;   ^punctuation.comment.eol.start
;     ^comment.gcode

N123 G1 X5 Y0 *64 ; EOL Comment
;<-entity.nword.gcode
;^constant.numeric.line-number.gcode
;    ^entity.command.gcode
;     ^constant.numeric.command.gcode
;       ^keyword.param.gcode
;        ^constant.numeric.param.gcode
;             ^punctuation.marker.checksum.gcode
;              ^constant.numeric.checksum.gcode

N234 G1 X-5 Y+2 *64 error
;<-entity.nword.gcode
;^constant.numeric.line-number.gcode
;    ^entity.command.gcode
;     ^constant.numeric.command.gcode
;       ^keyword.param.gcode
;        ^constant.numeric.param.gcode
;               ^punctuation.marker.checksum.gcode
;                ^constant.numeric.checksum.gcode
;                  ^invalid.error.syntax.gcode

N234 M107 *64     
;              ^-invalid.error.syntax.gcode

M92 E304.5:304.5:420:420:420:420 ; EOL Comment
;<-entity.command.gcode
;^constant.numeric.command.gcode
;   ^keyword.param.gcode
;    ^constant.numeric.param.gcode
;             ^constant.numeric.param.gcode
;                                ^punctuation.comment.eol.start
;                                 ^comment.gcode

" Vim syntax file
" Language:	Buildscript
" Maintainer:	Mike Buland :)
" Last Change:	2009 Dec 4

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

" Keywords
syn keyword Conditional if then else
syn keyword Loop        for do in
syn keyword Logic       not and or
syn keyword Statement   include set unset function target input condition requires rule profile auto config display type default cache global value return output allow action warning error notice local continue break all export tag
syn keyword Todo        TODO FIXME XXX
syn keyword Type        int string bool float version
syn keyword Constant    null true false file never always important normal hidden autogenerated filetime
syn keyword Builtins    files dirs matches replace regex execute unlink exists getMakeDeps toString targets fileName dirName

syn match TargetProcess /[a-zA-Z_][a-zA-Z0-9_]*:/he=e-1

syn cluster CommentGroup contains=Todo

syn match Special display contained "\\\(x\x\+\|\o\{1,3}\|.\|$\)"
syn match Special display contained "\\\(u\x\{4}\|U\x\{8}\)"

" TODO: do we want to end at end of line too?
syn region doubleString start=+"+ skip=+\\\\\|\\"+ end=+"+ contains=Special,CmdEx

syn region CmdEx start=+$(+ skip=+"\|\\)\|\\\\+ end=+)+

syn case ignore
syn match Numbers display transparent "\<\d\|\.\d" contains=Number,Float,OctalError,Octal
syn match Number display contained "\d\+\(u\=l\{0,2}\|ll\=u\)\>"
" Hex
syn match Number display contained "0x\x\+\(u\=l\{0,2}\|ll\=u\)\>"
syn match Octal display contained "0\o\+\(u\=l\{0,2}\|ll\=u\)\>" contains=cOctalZero
syn match OctalZero display contained "\<0"
syn match Float display contained "\d\+f"
syn match Float display contained "\d\+\.\d*\(e[-+]\=\d\+\)\=[fl]\="
syn match Float display contained "\.\d\+\(e[-+]\=\d\+\)\=[fl]\=\>"
syn match Float display contained "\d\+e[-+]\=\d\+[fl]\=\>"
" Flag bad digits in octal
syn match OctalError display contained "0\o*[89]\d*"
syn case match


syn region cppComment start="#" skip="\\$" end="$" contains=@CommentGroup keepend
syn region cComment start="/\*" end="\*/" contains=@CommentGroup fold
syn region cppComment start="//" skip="\\$" end="$" contains=@CommentGroup keepend


syntax region Block start="{" end="}" transparent fold

hi def link OctalError Error
hi def link cComment   Comment
hi def link cppComment Comment
hi def link singleString String
hi def link doubleString String
hi def link cmdEx String
hi def link Constructor Operator
hi def link Logic Statement
hi def link Loop Conditional
hi def link Builtins Function
hi def link TargetProcess Type


" vim syntax file
" Language: Replicated Object Notation 2.1 (open)
"
if exists("b:current_syntax")
    finish
endif

syn match COMMENT_ATOM /@\~/
hi COMMENT_ATOM ctermfg=Grey cterm=bold

syn match ERROR_ATOM /@\~\~\~\~\~\~\~\~\~\~/
hi ERROR_ATOM ctermfg=Red cterm=bold

syn match ANNOTATION /@\s*[0-9A-Za-z_\~]\{1,10}\>/
hi ANNOTATION ctermfg=Grey cterm=italic

syn match NAME_ATOM /\(>\s*\)\=\<[0-9A-Za-z_\~]\{1,10}\>/
hi NAME_ATOM ctermfg=Green cterm=underline,bold

syn match INT_ATOM /\(=\s*\)\=[+-]\=[0-9]\{1,19}\>/
hi INT_ATOM ctermfg=LightGreen

syn match VARIETY_NAME_ATOM /\(>\s*\)\=\<\([0-9A-F][/]\)[0-9A-Za-z_~]\{1,10}\>/
hi VARIETY_NAME_ATOM ctermfg=Green cterm=underline,bold

syn match FLOAT_ATOM /\(\^\s*\)\=[+-]\{0,1}[0-9]\{0,19}[.][0-9]\{1,19}\([Ee][+-]\{0,1}[0-9]\{1,3}\)\{0,1}\>/
hi FLOAT_ATOM ctermfg=Green

syn match UUID_ATOM /\(>\s*\)\=\<\([0-9A-F][/]\)\=[0-9A-Za-z_~]\{1,10}[-+$%][0-9A-Za-z_~]\{1,10}\>/
hi UUID_ATOM ctermfg=Green cterm=underline

syn match RM_CHAIN /([0-9]\{1,10})/
hi RM_CHAIN ctermfg=DarkRed cterm=bold

syn match STRING_CHAIN /"\(\\u[0-9a-fA-F]\{4}\|\\[^\n\r]\|[^'\\\n\r]\)*"/
hi STRING_CHAIN ctermfg=DarkGreen cterm=bold

syn match STRING_ATOM /'\(\\u[0-9a-fA-F]\{4}\|\\[^\n\r]\|[^'\\\n\r]\)*'/
hi STRING_ATOM ctermfg=DarkGreen

syn match OP_RM_ID  /@\s*\([0-9A-F][/]\)\=[0-9A-Za-z_~]\{1,10}[-][0-9A-Za-z_~]\{1,10}\>/
hi OP_RM_ID ctermfg=LightMagenta

syn match OP_ID  /@\s*\([0-9A-F][/]\)\=[0-9A-Za-z_~]\{1,10}[+][0-9A-Za-z_~]\{1,10}\>/
hi OP_ID ctermfg=Blue

syn match REF_ID /:\s*\([0-9A-F][/]\)\=[0-9A-Za-z_~]\{1,10}\([-+$%][0-9A-Za-z_~]\{1,10}\)\{0,1}\>/
hi REF_ID ctermfg=DarkBlue

syn match RM_REF_ID /:\s*\([0-9A-F][/]\)\=[0-9A-Za-z_~]\{1,10}[-][0-9A-Za-z_~]\{1,10}\>/
hi RM_REF_ID ctermfg=Magenta

syn match OPTERM /[!?,;]/
hi OPTERM ctermfg=Brown

syn match FRAMETERM /[.]\n/
hi FRAMETERM ctermfg=Red

let b:current_syntax = "ron"

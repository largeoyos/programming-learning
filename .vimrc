" ===== basic =====
let mapleader = " "

set number
set relativenumber
set cursorline
set wrap
set clipboard=unnamedplus
set expandtab
set autoindent
set smartindent
set tabstop=4
set shiftwidth=4
set hlsearch

" ===== insert auto-pairs =====
inoremap ( ()<ESC>i
inoremap [ []<ESC>i
inoremap { {}<ESC>i
inoremap < <><ESC>i
inoremap ' ''<ESC>i
inoremap " ""<ESC>i

" ===== normal mode =====
nnoremap H ^
nnoremap L $

nnoremap <leader>j 20j
nnoremap <leader>k 20k

nnoremap <leader>p "+p
nnoremap <leader>y "+y
nnoremap <leader>nl :nohlsearch<CR>

nnoremap <leader>w :w<CR>
nnoremap <leader>q :q<CR>
nnoremap <leader>x :x<CR>

nnoremap <leader>' j0i<BS><Esc>

nnoremap <leader>ac ggVG
nnoremap <leader>ay mzggVGy`z
nnoremap <leader>a<leader>y mzggVG"+y`z

" ===== insert mode =====
inoremap jj <Esc>

inoremap ,,s <sub></sub><Esc>5hi
inoremap ,,p <p></p><Esc>3hi

" ===== visual mode =====
xnoremap ,,s di<sub></sub><Esc>6hp
xnoremap <leader>8 di****<Esc>hhp
xnoremap <leader>9 di()<Esc>hp
xnoremap <leader>[ di[]<Esc>hp

(add-to-list 'load-path "~/.emacs.d/lisp/")
(setq backup-directory-alist (quote (("." . "~/.emacs_backups"))))

(require 'linum)
(global-linum-mode t)


(global-set-key "\C-h" 'delete-backward-char)
(global-set-key "\C-xh" 'help-command)
(global-set-key "\C-xl" 'goto-line)
(global-set-key "\C-xs" 'shell)
(global-set-key "\C-xg" 'gdb)
(global-set-key "\C-xr" 'rename-buffer)

(ido-mode 1)

(setq tab-width 4)
(setq default-tab-width 4)
(setq indent-tabs-mode nil)
(setq-default indent-tabs-mode nil) 
(setq c-basic-offset 4)

(setq default-fill-column 80)
(setq scroll-step 1
	  scroll-margin 3
	  scroll-conservatively 10000)


;; markdown
(autoload 'markdown-mode "markdown-mode"
  "Major mode for editing Markdown files" t)
(add-to-list 'auto-mode-alist '("\\.markdown\\'" . markdown-mode))
(add-to-list 'auto-mode-alist '("\\.md\\'" . markdown-mode))


;; C language language setting
(add-hook 'c-mode-hook
	  '(lambda ()
	     (c-set-style "K&R")
	     (setq tab-width 2)
	     (setq indent-tabs-mode nil)
	     (setq c-basic-offset 2)))


;; CC language setting
(add-hook 'c++-mode-hook
	  '(lambda ()
	     (setq tab-width 2)
	     (setq indent-tabs-mode nil)
	     (setq c-basic-offset 2)))


;; Perl language setting
(defalias 'perl-mode 'cperl-mode)
(add-to-list 'auto-mode-alist '("\\.psgi\\'" . cperl-mode))

(global-set-key (kbd "C-M-=") 'indent-dwim)
(global-set-key (kbd "C-;") 'comment-dwin)


;; (setq erlang-indent-level 2)


;; shell
(add-hook 'sh-mode-hook
		  '(lambda ()
			 (setq sh-basic-offset 2
				   sh-indentation 2
				   sh-indent-for-case-label 0
				   sh-indent-for-case-alt '+)))


;; java
(defun krt-java-mode-hook ()
  (setq indent-tabs-mode nil
		tab-width 2
		c-basic-offset 2))

(add-hook 'java-mode-hook 'krt-java-mode-hook)


;; evernote-mode
;; (setq evernote-developer-token "...")
;; (setq evernote-enml-formatter-command '("w3m" "-dump" "-I" "UTF8" "-O" "UTF8"))
;; (require 'evernote-mode)
;; (global-set-key "\C-cec" 'evernote-create-note)
;; (global-set-key "\C-ceo" 'evernote-open-note)
;; (global-set-key "\C-ces" 'evernote-search-notes)
;; (global-set-key "\C-ceS" 'evernote-do-saved-search)
;; (global-set-key "\C-cew" 'evernote-write-note)
;; (global-set-key "\C-cep" 'evernote-post-region)
;; (global-set-key "\C-ceb" 'evernote-browser)


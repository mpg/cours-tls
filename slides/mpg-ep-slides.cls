% personal class for Epita TLS slides [[2
%
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{mpg-ep-slides}


% use beamer with custom encoding, fonts and colors [[2
%
% add french and table (for xcolor via beamer) to global options
\edef\@classoptionslist{\@classoptionslist,french,table}
\LoadClass{beamer}
\usepackage[utf8]{inputenc}
\usepackage[T1]{fontenc}
\usepackage[sc, osf]{mathpazo}
\usepackage{textcomp, amssymb}
\renewcommand\sfdefault{iwona}
\rmfamily
\DeclareFontShape{T1}{pplj}{bx}{sc}{<->ssub*pplj/bx/n}{}

\definecolor{mstruct}{rgb}{0.7,0.0,0.0}

% beamer theme [[2
%
\usetheme[compress]{Singapore}
\usefonttheme[onlymath]{serif}
\usefonttheme{professionalfonts}
\setbeamercolor{structure}{fg=mstruct}
\setbeamertemplate{navigation symbols}{}
% pour avoir les vignettes de Singapore sans sous-sections
\setcounter{subsection}{1}
\usepackage{remreset}
\makeatletter \@removefromreset{subsection}{section} \makeatother

% load various useful packages [[2
%
\usepackage{mathtools, mathstyle}
\usepackage{ifmtarg, fixltx2e, xargs, xspace, cmdstring, ted, etex}
\usepackage{booktabs, fancyvrb}
\usepackage{tikz}
\usetikzlibrary{shapes,arrows,positioning,fit,decorations.pathmorphing,shapes.misc}
\usepackage{babel}

% frenchb (+xspace) settings [[2
%
\frenchbsetup{og=«,fg=»,
  AutoSpacePunctuation=false,
  ThinColonSpace=true,
  SuppressWarning=true,
}

% nbsp treated as a normal space (to let frenchb do its job)
\DeclareUnicodeCharacter{00A0}{ }

% don't eat space before high punctuation
\xspaceremoveexception{:}
\xspaceremoveexception{;}
\xspaceremoveexception{?}
\xspaceremoveexception{!}

% déclaration du format 3 pages sur une [[2
%
\usepackage{pgfpages}
\pgfpagesdeclarelayout{3 on 1}{%
  \def\pgfpageoptionborder{0pt}%
  }{%
  \pgfpagesphysicalpageoptions{%
    logical pages=3,%
    physical height=\pgfpageoptionheight,%
    physical width=\pgfpageoptionwidth}
  \pgfpageslogicalpageoptions{1}{%
    resized width=.66\pgfphysicalwidth,%
    resized height=.33\pgfphysicalheight,%
    border shrink=\pgfpageoptionborder,%
    center=\pgfpoint{.5\pgfphysicalwidth}{.83\pgfphysicalheight}}%
  \pgfpageslogicalpageoptions{2}{%
    resized width=.66\pgfphysicalwidth,%
    resized height=.33\pgfphysicalheight,%
    border shrink=\pgfpageoptionborder,%
    center=\pgfpoint{.5\pgfphysicalwidth}{.50\pgfphysicalheight}}%
  \pgfpageslogicalpageoptions{3}{%
    resized width=.66\pgfphysicalwidth,%
    resized height=.33\pgfphysicalheight,%
    border shrink=\pgfpageoptionborder,%
    center=\pgfpoint{.5\pgfphysicalwidth}{.17\pgfphysicalheight}}}

% choix du format [[2
%
\makeatletter
\@ifundefined{nup}{\def\nup{1}}{%
  \colorlet{mspecial}{black}
  \colorlet{mstruct}{black}
  \colorlet{mcode}{black}
  \newlength\hackpaperwidth \hackpaperwidth210mm
  \newlength\hackpaperheight \hackpaperheight297mm
  \RequirePackage{everyshi}
  \ifnum\nup=4
    \EveryShipout{%
      \special{papersize=\the\hackpaperheight,\the\hackpaperwidth}}
  \else
    \EveryShipout{%
      \special{papersize=\the\hackpaperwidth,\the\hackpaperheight}}
  \fi
  }
\makeatother
\ifcase\nup \or % 1
\or
  \pgfpagesuselayout{2 on 1}[a4paper, border shrink=1cm]
\or
  \pgfpagesuselayout{3 on 1}[a4paper, border shrink=1cm]
\or 
  \pgfpagesuselayout{4 on 1}[a4paper, border shrink=1cm, landscape]
\fi


% theorems & Co [[2
%
\newtheorem{thm}{Théorème}
\newtheorem{prop}{Proposition}
\newtheorem{coro}{Corollaire}
\newtheorem{tdef}{Définition}
\let\fact\relax
\newtheorem{fact}{Fait}
\newtheorem{lem}{Lemme}

% numbering [[2
%
\mathtoolsset{showonlyrefs,showmanualtags}

% tocs [[2
%
\newcommand \tocsect {%
  \frame{\tableofcontents[currentsection]}
}
\newcommand \toc {%
  \frame{\tableofcontents}
}


% [[1
% vim: ft=tex fdm=marker fmr=[[,]] fdl=1 spell spelllang=fr,en

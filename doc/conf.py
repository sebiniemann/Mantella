# -*- coding: utf-8 -*-

#
# Project information
#

project = u'Mantella'
version = u'0.70'

master_doc = 'index'
source_suffix = ['.rst']

pygments_style = 'sphinx'
extensions = [
  'sphinx.ext.mathjax',
  'sphinx.ext.githubpages',
]

#
# General configuration
#

nitpicky = True
nitpick_ignore = [
  ('cpp:type', 'T'),
  ('cpp:type', 'T1'),
  ('cpp:type', 'T2'),
  ('cpp:type', 'T3'),
  ('cpp:type', 'N'),
  ('cpp:type', 'std'),
  ('cpp:type', 'std::size_t'),
  ('cpp:type', 'std::array'),
  ('cpp:type', 'std::vector'),
  ('cpp:type', 'std::function'),
  ('cpp:type', 'parameter'),
  ('cpp:type', 'evaluations'),
  ('cpp:type', 'acceptable_deviation'),
  ('cpp:type', 'std::chrono'),
  ('cpp:type', 'std::chrono::nanoseconds'),
  ('cpp:type', 'initial_parameters'),
  ('cpp:type', 'maximal_distance'),
  ('cpp:type', 'minimal_distance'),
  ('cpp:type', 'active_dimensions'),
]

add_function_parentheses = False

#
# Options for HTML output
#

html_theme = "mantella"
html_theme_path = ["_themes"]
---
layout: startpage
title:  Mantella
---

Right now, this is a little playground while I revise the repo and try out different formats :)

<ul class="articleList">
  {% for post in site.posts %}
    <li class="item">
      <div class="category">
        {{ post.category }}
      </div>
      <div class="title">
        <a href="{{ site.baseurl }}{{ post.url }}">{{ post.title }}</a>
      </div>
      <div class="intro">{{ post.intro }}</div>
    </li>
  {% endfor %}
</ul>
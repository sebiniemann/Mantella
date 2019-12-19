<template>
  <div>
    <header>
      <span class="toogle" @click="toogleMenu">
        <span v-if="showMenu">Close</span>
        <span v-else>Menu</span>
      </span>
      Salp
    </header>
    <nav v-if="showMenu">
      <ul>
        <li>
          <c-link href="/">Home</c-link>
        </li>
        <li>
          <c-link href="/get-started.html">Get Started</c-link>
        </li>
        <li>
          <c-link href="/docs.html">Documentation</c-link>
        </li>
      </ul>
    </nav>
    <main>
      <router-view></router-view>
    </main>
  </div>
</template>

<script>
export default {
  data() {
    return {
      showMenu: false,
    };
  },

  methods: {
    toogleMenu() {
      this.showMenu = !this.showMenu;
    },
  },
}
</script>

<style>
code {
  background: #f3f6fa;
  border: 1px solid #dce6f0;
  border-radius: 6px;
  display: block;
  padding-left: 16px;
  padding-right: 16px;
  white-space: pre;
}

.toogle {
  cursor: pointer;
}
</style>

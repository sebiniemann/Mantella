<template>
  <a
    v-if="isExternal(href)"
    :href="href"
    target="_blank"
    rel="noopener noreferrer"
  >
    <slot></slot>
  </a>
  <router-link v-else :to="href">
    <slot></slot>
  </router-link>
</template>

<script>
export default {
  props: {
    href: { type: String, required: true },
  },

  methods: {
    isExternal(href) {
      return /^[a-z]+?:/.test(href) || !/(?:\/|\.html)$/.test(href);
    },
  },
};
</script>

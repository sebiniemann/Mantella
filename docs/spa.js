import Vue from 'vue';
import VueRouter from 'vue-router';

import cLink from './components/cLink';
import cExample from './components/cExample';
import Layout from './Layout.vue';
import routes from './routes';

Vue.component('cExample', cExample);
Vue.component('cLink', cLink);

Vue.use(VueRouter);
const router = new VueRouter({
  mode: 'history',
  routes,
});

new Vue({
  router,
  render: h => h(Layout),
}).$mount('#spa');

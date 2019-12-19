import page1 from './pages/index.vue';
import page2 from './pages/docs.vue';
import page3 from './pages/get-started.vue';

export default [{
  path: '/',
  component: page1,
}, {
  path: '/docs.html',
  component: page2,
}, {
  path: '/get-started.html',
  component: page3,
}];

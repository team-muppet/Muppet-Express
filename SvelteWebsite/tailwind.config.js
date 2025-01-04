/** @type {import('tailwindcss').Config} */

import { skeleton, contentPath } from '@skeletonlabs/skeleton/plugin';
import * as themes from '@skeletonlabs/skeleton/themes';
import forms from '@tailwindcss/forms';

export default {
  darkMode: 'selector',
  content: [
    './src/**/*.{html,js,svelte,ts}',
    contentPath(import.meta.url, 'svelte')
  ],

  theme: {
    extend: {}
  },

  plugins: [
    forms,
    skeleton({
      // NOTE: each theme included will increase the size of your CSS bundle
      themes: [themes.cerberus, themes.legacy]
    })
  ]
};

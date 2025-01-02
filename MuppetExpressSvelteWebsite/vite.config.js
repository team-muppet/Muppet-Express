import { purgeCss } from 'vite-plugin-tailwind-purgecss';
import { sveltekit } from '@sveltejs/kit/vite';
import { defineConfig } from 'vite';

export default defineConfig({
	plugins: [sveltekit(), purgeCss()],
	server: {
		proxy: {
			// Forward API requests starting with '/api' to your C++ server
			'/api': {
				target: 'http://localhost:2000', // Replace with your C++ server's port
				changeOrigin: true,
				rewrite: (path) => path.replace(/^\/api/, '') // Remove '/api' prefix when forwarding
			}
		}
	}
});

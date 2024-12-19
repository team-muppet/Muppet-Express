import { purgeCss } from 'vite-plugin-tailwind-purgecss';
import { sveltekit } from '@sveltejs/kit/vite';
import { defineConfig } from 'vite';

export default defineConfig({
	plugins: [sveltekit(), purgeCss()],
	server: {
		proxy: {
			// Proxy any requests that are not matched by Svelte
			'/': {
				target: 'http://localhost:2000', // Replace with your C++ server's URL
				changeOrigin: true,
				bypass: (req) => {
					const knownFrontendRoutes = ['/about', '/contact', '/'];

					// Check if the request URL matches any known frontend routes
					if (knownFrontendRoutes.some((route) => req.url?.includes(route))) {
						return null; // Let Svelte handle these routes
					}

					// Forward any other requests to the backend server
				}
			}
		}
	}
});

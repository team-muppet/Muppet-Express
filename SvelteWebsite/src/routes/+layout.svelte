<script>
	import "../app.css";
	import { AppBar } from "@skeletonlabs/skeleton-svelte";
	import { onMount } from "svelte";
	import "svelte-jsoneditor/themes/jse-theme-dark.css";
	import LightSwitch from "../components/LightSwitch.svelte";
	import { ToastProvider } from "@skeletonlabs/skeleton-svelte";
	import Icon from "@iconify/svelte";

	let { children } = $props();

	function updateThemeClass() {
		if (document.documentElement.classList.contains("dark")) {
			// Add "jse-theme-dark" only if it's not already present
			if (!document.documentElement.classList.contains("jse-theme-dark")) {
				document.documentElement.classList.add("jse-theme-dark");
			}
		} else {
			// Remove "jse-theme-dark" only if it's present
			if (document.documentElement.classList.contains("jse-theme-dark")) {
				document.documentElement.classList.remove("jse-theme-dark");
			}
		}
	}

	onMount(() => {
		// Initialize the theme class on load
		updateThemeClass();

		const observer = new MutationObserver(updateThemeClass);

		observer.observe(document.documentElement, { attributes: true, attributeFilter: ["class"] });

		return () => {
			observer.disconnect();
		};
	});
</script>

<ToastProvider>
	<div class="grid grid-rows-[auto_1fr_auto]">
		<!-- Header -->
		<header class="border-surface-500/20 bg-surface-50-950 w-full border-b-[1px] p-4 py-3 xl:px-10">
			<div class="container mx-auto grid max-w-screen-xl grid-cols-[auto_1fr_auto] xl:grid-cols-[1fr_auto_1fr] items-center gap-4">
				<div class="flex gap-4 items-center">
					<strong class="text-xl mr-2">Muppet Express</strong>
					<a class="hover:underline flex items-center gap-1" href="/" target="_self">
						<Icon icon="material-symbols:api" />
						Endpoints
					</a>
					<a class="hover:underline flex items-center gap-1" href="/pokemon" target="_self">
						<Icon icon="ic:baseline-catching-pokemon" />
						Pokémon
					</a>
				</div>
				<div class="flex gap-4"></div>
				<div class="flex justify-end">
					<LightSwitch />
				</div>
			</div>
		</header>
		<!-- Page -->
		<div class="container mx-auto max-w-screen-xl">
			<!-- Main -->
			<main class="">
				{@render children?.()}
			</main>
		</div>
	</div>
</ToastProvider>

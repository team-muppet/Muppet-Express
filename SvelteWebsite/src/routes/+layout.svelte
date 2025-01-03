<script>
	import '../app.css';
	import { AppBar } from "@skeletonlabs/skeleton-svelte";
	import { onMount } from "svelte";
	import "svelte-jsoneditor/themes/jse-theme-dark.css";
	import LightSwitch from "../components/LightSwitch.svelte";

	let { children } = $props();

	function updateThemeClass() {
		if (document.documentElement.classList.contains("dark")) {
			document.body.classList.add("jse-theme-dark");
		} else {
			document.body.classList.remove("jse-theme-dark");
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

<div class="grid grid-rows-[auto_1fr_auto]">
	<!-- Header -->
	<header class="">
		<AppBar>
			{#snippet trail()}
				<LightSwitch />
			{/snippet}

			<strong class="text-xl">Muppet Express</strong>
		</AppBar>
	</header>
	<!-- Page -->
	<div class="container mx-auto">
		<!-- Main -->
		<main class="space-y-4 p-4">
			{@render children?.()}
		</main>
	</div>
</div>

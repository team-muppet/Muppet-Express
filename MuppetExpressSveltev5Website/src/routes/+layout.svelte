<script>
	import "../app.postcss";
	import { AppBar } from "@skeletonlabs/skeleton-svelte";
	import { onMount } from "svelte";

	// Highlight JS
	import hljs from "highlight.js/lib/core";
	import "highlight.js/styles/github-dark.css";
	import { storeHighlightJs } from "@skeletonlabs/skeleton";
	import xml from "highlight.js/lib/languages/xml"; // for HTML
	import css from "highlight.js/lib/languages/css";
	import javascript from "highlight.js/lib/languages/javascript";
	import typescript from "highlight.js/lib/languages/typescript";
	import json from 'highlight.js/lib/languages/json';
	import 'svelte-jsoneditor/themes/jse-theme-dark.css';

	hljs.registerLanguage("xml", xml); // for HTML
	hljs.registerLanguage("css", css);
	hljs.registerLanguage("javascript", javascript);
	hljs.registerLanguage("typescript", typescript);
	hljs.registerLanguage('json', json);
	storeHighlightJs.set(hljs);

	// Floating UI for Popups
	import { computePosition, autoUpdate, flip, shift, offset, arrow } from "@floating-ui/dom";
	import { storePopup } from "@skeletonlabs/skeleton";
    import LightSwitch from "../components/LightSwitch.svelte";
	/** @type {{children?: import('svelte').Snippet}} */
	let { children } = $props();
	storePopup.set({ computePosition, autoUpdate, flip, shift, offset, arrow });

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

<!-- App Bar -->
<AppBar>
	{#snippet lead()}
		
			<strong class="text-xl">Muppet Express</strong>
		
		{/snippet}
	{#snippet center()}
		
			<strong class="text-xl">Test</strong>
			
		
		{/snippet}
	{#snippet trail()}
		
			<LightSwitch/>
		
		{/snippet}
</AppBar>

<!-- Page Route Content -->
{@render children?.()}

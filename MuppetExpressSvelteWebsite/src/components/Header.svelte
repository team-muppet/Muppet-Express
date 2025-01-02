<script>
	import { onMount } from "svelte";
	import { page } from "$app/stores";
	import logo from "$lib/images/logo_wide.svg";
	import LogoWide from "./LogoWide.svelte";
	import LightSwitch from "./LightSwitch.svelte";

	let previousScroll = 0;
	let headerVisible = $state(true);
	let isMenuOpen = $state(false);
	let isDarkMode = false;

	const navLinks = [
		{ title: "Velkommen", url: "/" },
		{ title: "Info til lejere", url: "/tenants" },
		{ title: "Om mig", url: "/about" },
	];

	const handleScroll = () => {
		const currentScroll = window.scrollY;
		if (currentScroll > previousScroll && currentScroll > 50) {
			headerVisible = false;
		} else if (currentScroll < previousScroll) {
			headerVisible = true;
		}
		previousScroll = currentScroll;
	};

	onMount(() => {
		window.addEventListener("scroll", handleScroll);

		// Initialize isDarkMode
		isDarkMode = document.documentElement.classList.contains("dark");

		const observer = new MutationObserver(() => {
			isDarkMode = document.documentElement.classList.contains("dark");
		});

		observer.observe(document.documentElement, { attributes: true, attributeFilter: ["class"] });

		return () => {
			window.removeEventListener("scroll", handleScroll);
			observer.disconnect();
		};
	});
</script>

<header
	class={`fixed top-0 left-0 w-full z-50 transition-transform duration-500 bg-white bg-opacity-95 dark:bg-gray-900 dark:bg-opacity-95 ${headerVisible ? "translate-y-0" : "-translate-y-full"}`}>
	<div class="container mx-auto flex items-center justify-between px-4 py-3 max-w-screen-xl relative">
		<!-- Burger menu button for small screens -->
		<div class="md:hidden order-1">
			<button class="burger-menu" onclick={() => (isMenuOpen = !isMenuOpen)}>
				<div class="line bg-gray-800 dark:bg-white"></div>
				<div class="line bg-gray-800 dark:bg-white"></div>
				<div class="line bg-gray-800 dark:bg-white"></div>
			</button>
		</div>

		<!-- Logo -->
		<a href="/" class="order-2 mx-auto md:mx-0 md:ml-2">
			<div class="logo">
				<div class="img">
					<LogoWide />
				</div>
			</div>
		</a>

		<!-- Icons -->
		<div class="corner flex items-center space-x-4 order-3">
			<LightSwitch />
		</div>

		<!-- Nav links for desktop -->
		<nav class="nav-desktop hidden md:flex space-x-6 items-center absolute left-1/2 transform -translate-x-1/2 order-4">
			{#each navLinks as link}
				<a
					href={link.url}
					class="nav-link text-gray-800 dark:text-white hover:text-theme-1 transition-colors"
					aria-current={$page.url.pathname === link.url ? "page" : undefined}>
					{link.title}
				</a>
			{/each}
		</nav>
	</div>

	<!-- Mobile nav menu -->
	{#if isMenuOpen}
		<nav class="list-nav bg-white dark:bg-gray-800 absolute top-full left-0 w-full md:hidden">
			<ul class="flex flex-col items-center">
				{#each navLinks as link}
					<li>
						<a
							href={link.url}
							onclick={() => (isMenuOpen = false)}
							aria-current={$page.url.pathname === link.url ? "page" : undefined}
							class="flex items-center justify-center space-x-3 px-4 py-4 text-gray-800 dark:text-white hover:bg-gray-100 dark:hover:bg-gray-700 w-full">
							<span class="text-lg font-semibold">{link.title}</span>
						</a>
					</li>
				{/each}
			</ul>
		</nav>
	{/if}
</header>

<style>
	.burger-menu {
		display: flex;
		flex-direction: column;
		justify-content: space-between;
		width: 30px;
		height: 22px;
		cursor: pointer;
	}

	.line {
		width: 100%;
		height: 2px;
	}

	.logo .img {
		height: 4em;
	}

	.nav-link {
		text-transform: uppercase;
		font-weight: 700;
		letter-spacing: 0.1em;
		transition: color 0.3s;
	}
</style>

<script>
  import { onMount } from "svelte";
  import Icon from "@iconify/svelte";
  import { modeOsPrefers, modeUserPrefers, modeCurrent, setInitialClassState } from "@skeletonlabs/skeleton";

  let isFirstLoad = $state(true);

  // Initialize the theme on page load
  onMount(() => {
    // Sync the theme with the user's preference or OS preference
    if (!("modeCurrent" in localStorage)) {
      modeCurrent.set($modeOsPrefers); // Use $modeOsPrefers to get the boolean value
    }
    // Set isFirstLoad to true on initial mount
    isFirstLoad = true;
  });

  // Reactive variable to determine the current mode
  let isDarkMode = $derived(!$modeCurrent); // $modeCurrent is true for light mode

  // Toggle the theme
  function toggleTheme() {
    const newMode = !$modeCurrent; // $modeCurrent is a boolean
    modeUserPrefers.set(newMode);
    modeCurrent.set(newMode);

    // Update the class on the document root
    const elemHtmlClasses = document.documentElement.classList;
    const classDark = "dark";
    newMode ? elemHtmlClasses.remove(classDark) : elemHtmlClasses.add(classDark);

    // After the first toggle, set isFirstLoad to false
    isFirstLoad = false;
  }
</script>

<svelte:head>
  <!-- Initialize the theme on page load -->
  {@html `<script>(${setInitialClassState.toString()})();</script>`}
</svelte:head>

<!-- Button to toggle the theme -->
<button onclick={toggleTheme} aria-label="Toggle light or dark mode" class="focus:outline-none group">
  {#if isDarkMode}
    <!-- Dark Mode -->
    {#if isFirstLoad}
      <!-- First load, show static moon icon -->
      <Icon icon="line-md:moon" class="text-3xl text-gray-800 dark:text-white" />
    {:else}
      <!-- After toggle, show transition icon -->
      <Icon icon="line-md:sunny-outline-to-moon-transition" class="text-3xl text-gray-800 dark:text-white" />
    {/if}
  {:else}
    <!-- Light Mode -->
    {#if isFirstLoad}
      <!-- First load, show static sun icon -->
      <Icon icon="line-md:sunny-outline" class="text-3xl text-gray-800 dark:text-white" />
    {:else}
      <!-- After toggle, show transition icon -->
      <Icon icon="line-md:moon-to-sunny-outline-transition" class="text-3xl text-gray-800 dark:text-white" />
    {/if}
  {/if}
</button>

<style>
  button {
    background: none;
    border: none;
    cursor: pointer;
  }
</style>

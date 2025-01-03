<script>
  import { onMount, onDestroy } from 'svelte';
  import Icon from '@iconify/svelte';

  let theme = $state(null);      // 'light' | 'dark' or null if unset
  let isFirstLoad = $state(true);
  let mediaQueryList = $state();    // will hold the (prefers-color-scheme: dark) MediaQueryList
  let listener = $state();          // reference to the event listener function

  onMount(() => {
    // 1) Read user preference from localStorage
    const storedTheme = localStorage.getItem('theme');
    if (storedTheme) {
      // User already made a choice
      theme = storedTheme;
    } else {
      // Follow OS preference
      mediaQueryList = window.matchMedia('(prefers-color-scheme: dark)');
      theme = mediaQueryList.matches ? 'dark' : 'light';

      // 2) If no user preference is set, track OS changes
      listener = (e) => {
        // only sync with OS if the user STILL hasn't chosen a theme
        if (!localStorage.getItem('theme')) {
          theme = e.matches ? 'dark' : 'light';
          applyThemeClass(theme);
        }
      };
      mediaQueryList.addEventListener('change', listener);
    }

    // Apply the chosen/OS theme class initially
    applyThemeClass(theme);

    // Mark first load complete
    isFirstLoad = false;
  });

  onDestroy(() => {
    // Clean up event listener if we attached one
    if (mediaQueryList && listener) {
      mediaQueryList.removeEventListener('change', listener);
    }
  });

  // Update <html> with or without 'dark' class
  function applyThemeClass(themeValue) {
    const htmlClasses = document.documentElement.classList;
    if (themeValue === 'dark') {
      htmlClasses.add('dark');
    } else {
      htmlClasses.remove('dark');
    }
  }

  // User toggles theme
  function toggleTheme() {
    // Flip light/dark
    theme = theme === 'dark' ? 'light' : 'dark';

    // Apply new theme and store preference
    applyThemeClass(theme);
    localStorage.setItem('theme', theme);
  }
</script>

<!-- Toggle button with dynamic icons -->
<button onclick={toggleTheme} aria-label="Toggle dark/light mode">
  {#if theme === 'dark'}
    {#if isFirstLoad}
      <!-- First load: show static moon icon -->
      <Icon icon="line-md:moon" class="text-3xl" />
    {:else}
      <!-- After toggling: transition from sun -> moon -->
      <Icon icon="line-md:sunny-outline-to-moon-transition" class="text-3xl" />
    {/if}
  {:else if isFirstLoad}
    <!-- Light mode (on first load) -->
    <Icon icon="line-md:sunny-outline" class="text-3xl" />
  {:else}
    <!-- After toggling: transition from moon -> sun -->
    <Icon icon="line-md:moon-to-sunny-outline-transition" class="text-3xl" />
  {/if}
</button>

<style>
  button {
    background: none;
    border: none;
    cursor: pointer;
  }
</style>

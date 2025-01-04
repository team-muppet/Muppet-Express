<script>
	import ApiCallButton from "../components/ApiCallButton.svelte";
	import Test from "../components/Test.svelte";

	// Example API call configurations
	const apiCalls = [
		{
			title: "Test",
			method: "GET",
			endpoint: "/api/test",
			headers: null,
			body: null,
		},
		{
			title: "Echo",
			method: "POST",
			endpoint: "/api/echo",
			headers: { "Content-Type": "application/json" },
			body: { Test: "Test" },
		},
		{
			title: "Fish",
			method: "GET",
			endpoint: "/api/fish",
			headers: null,
			body: null,
		},
		{
			title: "Fish",
			method: "GET",
			endpoint: "/api/fish/1/1",
			headers: null,
			body: null,
		},
	];
</script>

<svelte:head>
	<title>Muppet Express</title>
</svelte:head>

<div class="container mx-auto h-screen flex justify-center p-4">
	<div class="w-full space-y-8 text-center">
		<h2 class="text-4xl font-bold">Welcome to Muppet Express Showcase</h2>

		<!-- Loop through apiCalls to render multiple ApiCallButton components -->
		<div class="space-y-6">
			{#each apiCalls as apiCall}
				<ApiCallButton {...apiCall} />
			{/each}
		</div>
	</div>
</div>

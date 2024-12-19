<script>
	import ApiCallButton from "../components/ApiCallButton.svelte";

	// Example API call configurations
	const apiCalls = [
		{
			title: "Get Users",
			endpoint: "/api/users",
			headers: { "Content-Type": "application/json" },
			body: null,
		},
		{
			title: "Create User",
			endpoint: "/api/users/create",
			headers: { "Content-Type": "application/json" },
			body: { name: "John Doe", email: "john@example.com" },
		},
		{
			title: "Delete User",
			endpoint: "/api/users/delete",
			headers: { "Content-Type": "application/json" },
			body: { userId: 1 },
		},
	];
</script>

<div class="container mx-auto h-full flex justify-center items-center p-4">
	<div class="w-full max-w-3xl space-y-6 text-center">
		<h2 class="h2">Welcome to Skeleton API Showcase</h2>

		<p class="text-lg mb-6">Click the buttons below to perform different API calls and see the responses.</p>

		<!-- Loop through apiCalls to render multiple ApiCallButton components -->
		{#each apiCalls as { title, endpoint, headers, body }}
			<div class="p-4 border border-gray-300 rounded-lg bg-gray-50">
				<ApiCallButton {title} {endpoint} {headers} {body} />
			</div>
		{/each}
	</div>
</div>

<style lang="postcss">
	.container {
		@apply flex flex-col justify-center items-center h-full;
	}

	h2 {
		@apply text-3xl font-bold mb-4;
	}

	.p-4 {
		@apply shadow-md;
	}
</style>

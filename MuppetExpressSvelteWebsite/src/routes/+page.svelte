<script>
	import ApiCallButton from '../components/ApiCallButton.svelte';
    import Test from '../components/Test.svelte';
  
	// Example API call configurations
	const apiCalls = [
	  {
		title: 'Get Users',
		endpoint: '/api/users',
		headers: { 'Content-Type': 'application/json' },
		body: null,
	  },
	  {
		title: 'Create User',
		endpoint: '/api/users/create',
		headers: { 'Content-Type': 'application/json' },
		body: { name: 'John Doe', email: 'john@example.com' },
	  },
	  {
		title: 'Delete User',
		endpoint: '/api/users/delete',
		headers: { 'Content-Type': 'application/json' },
		body: { userId: 1 },
	  },
	];
  </script>
  
  <div class="container mx-auto h-screen flex justify-center p-4">
	<div class="w-full max-w-5xl space-y-8 text-center">
	  <h2 class="text-4xl font-bold">Welcome to Muppet Express Showcase</h2>
  
	  <!-- Loop through apiCalls to render multiple ApiCallButton components -->
	  <div class="space-y-6 ">
		<!-- {#each apiCalls as { title, endpoint, headers, body }}
		<ApiCallButton {title} {endpoint} {headers} {body} />
		<Test/>
		{/each} -->
		<Test/>
		<Test/>
		<Test/>
	  </div>
	</div>
  </div>
  
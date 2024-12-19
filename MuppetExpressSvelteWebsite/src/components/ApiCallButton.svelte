<script>
    import { writable } from 'svelte/store';
  
    // Props for configuring the component
    export let title = 'Make API Call';
    export let endpoint = '/api/example';
    export let headers = { 'Content-Type': 'application/json' };
    export let body = null; // Can be set to any JSON payload
  
    // State variables for response
    const responseCode = writable(null);
    const responseData = writable(null);
    const errorMessage = writable(null);
  
    // Function to make the API call
    async function makeApiCall() {
      responseCode.set(null);
      responseData.set(null);
      errorMessage.set(null);
  
      try {
        const response = await fetch(endpoint, {
          method: 'POST', // Change to 'GET', 'PUT', or 'DELETE' as needed
          headers: headers,
          body: body ? JSON.stringify(body) : null,
        });
  
        responseCode.set(response.status);
        const data = await response.json();
        responseData.set(data);
      } catch (error) {
        errorMessage.set(error.message);
      }
    }
  </script>
  
  <div class="space-y-4">
    <!-- Button to trigger the API call -->
    <button type="button" class="btn variant-filled" on:click={makeApiCall}>
      {title}
    </button>
  
    <!-- Display response code -->
    {#if $responseCode !== null}
      <div class="p-4 border border-gray-300 rounded-lg">
        <p><strong>Response Code:</strong> {$responseCode}</p>
      </div>
    {/if}
  
    <!-- Display response data -->
    {#if $responseData}
      <div class="p-4 border border-gray-300 rounded-lg bg-gray-100">
        <p><strong>Response Data:</strong></p>
        <pre class="whitespace-pre-wrap text-sm">{JSON.stringify($responseData, null, 2)}</pre>
      </div>
    {/if}
  
    <!-- Display error message -->
    {#if $errorMessage}
      <div class="p-4 border border-red-500 rounded-lg bg-red-50 text-red-700">
        <p><strong>Error:</strong> {$errorMessage}</p>
      </div>
    {/if}
  </div>
  
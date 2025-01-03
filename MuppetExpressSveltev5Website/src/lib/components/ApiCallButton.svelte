<script>
  import { JSONEditor } from "svelte-jsoneditor";
  import { TabGroup, Tab } from "@skeletonlabs/skeleton";
  import Icon from "@iconify/svelte";

  /*   // Props for configuring the component
  let { title, endpoint, headers, body } = $props({
    title: "Make API Call",
    endpoint: "/api/example",
    headers: { "Content-Type": "application/json" },
    body: { hello: "world" },
  }); */

    

  let {
    title = "Make API Call",
    endpoint = "/api/example",
    headers = { "Content-Type": "application/json" },
    body = {
      text: undefined,
      json: {
        string: "Hello World",
      },
    },
  } = $props();

  let body2 = $state({
    text: undefined,
    json: {
      string: "Hello World",
    },
  });

  // State variables for response
  let responseCode = $state(null);
  let responseData = $state(null);
  let errorMessage = $state(null);

  let responseColor = $derived(responseCode >= 200 && responseCode < 300 ? "#28a745" : "#dc3545");



  // Function to make the API call
  async function makeApiCall() {
    responseCode.set(null);
    responseData.set(null);
    errorMessage.set(null);

    try {
      const response = await fetch(endpoint, {
        method: "POST", // Change to 'GET', 'PUT', or 'DELETE' as needed
        headers,
        body: JSON.stringify(body),
      });

      responseCode.set(response.status);
      const data = await response.json();
      responseData.set(data);
    } catch (error) {
      errorMessage.set(error.message);
    }
  }

  // Handlers for JSON changes
  function handleBodyChange({ json }) {
    body = json;
  }

  function handleHeaderChange({ json }) {
    headers = json;
  }

  // Tab state
  let tabSet = $state(1);
</script>

<div class="card p-4 text-left space-y-4">
  <!-- Endpoint and API Call Button -->
  <div class="input-group input-group-divider grid-cols-[auto_1fr_auto] items-center">
    <div class="input-group-shim bg-inherit">
      <Icon icon="mdi:send" class="text-secondary-500" />
    </div>
    <input type="text" bind:value={endpoint} class="form-input" placeholder="Endpoint URL" />
    <button type="button" class="btn variant-filled-secondary" onclick={makeApiCall}>
      {title}
    </button>
  </div>

  <!-- Skeleton UI Tabs for Body and Headers -->
  <TabGroup>
    <Tab bind:group={tabSet} name="tabRequest" value={1}>
      <div class="flex items-start">
        <Icon icon="mdi:send" class="size-6 mr-2 text-secondary-500" />
        <span>Request</span>
      </div>
    </Tab>

    <Tab bind:group={tabSet} name="tabResponse" value={2}>
      <div class="flex items-start">
        <Icon icon="mdi:code-tags" class="size-6 mr-2" style="color: {responseColor}" />
        <span>Response</span>
      </div>
    </Tab>

    <!-- Panels -->
    <svelte:fragment slot="panel">
      TabSet = {tabSet}
      {#if tabSet == 1}
        <div class="grid grid-cols-2 gap-4">
          <div>
            <h3>Body</h3>
            <div class="mt-2" style="height: 200px;">
              <JSONEditor mode="text" mainMenuBar={false} statusBar={false} navigationBar={false} bind:content={body2} />
            </div>
          </div>

          <div>
            <h3>Header</h3>
            <div class="mt-2" style="height: 200px;">
              <JSONEditor mode="text" mainMenuBar={false} statusBar={false} navigationBar={false} content={{ json: headers }} on:change={handleHeaderChange} />
            </div>
          </div>
        </div>
      {:else if tabSet == 2}
        <div>
          {#if $responseCode}
            <span class="badge badge-success">
              Status: {$responseCode}
            </span>
          {/if}
          {#if $responseData}
            <pre class="bg-gray-100 p-3 rounded overflow-auto">{JSON.stringify($responseData, null, 2)}</pre>
          {/if}
          {#if $errorMessage}
            <p class="text-error mt-2">Error: {$errorMessage}</p>
          {/if}
        </div>
      {/if}
    </svelte:fragment>
  </TabGroup>
</div>

<style>
  .badge-success {
    background-color: #d4edda;
    color: #155724;
    padding: 0.25rem 0.5rem;
    border-radius: 4px;
    font-weight: bold;
  }
</style>

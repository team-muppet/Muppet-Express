<script lang="ts">
  import { JSONEditor, Mode } from "svelte-jsoneditor";
  import { Tabs } from "@skeletonlabs/skeleton-svelte";
  import Icon from "@iconify/svelte";

  let { title = "Make API Call", method = "GET", endpoint = "/api/example", headers = null, body = null } = $props();

  let methodInternal = $state(method);
  let endpointInternal = $state(endpoint);

  let headersJson = $state({
    text: headers ? JSON.stringify(headers) : "",
  });

  let bodyJson = $state({
    text: body ? JSON.stringify(body) : "",
  });

  // Tab state
  let group = $state("request");

  // State variables for response
  let response: Response = $state(null);
  let responseBody = $state(null);
  let errorMessage = $state(null);

  let responseHeadersJson = $derived(!response ? { text: "" } : { json: Object.fromEntries(response.headers) });
  let responseBodyJson = $derived({ text: !responseBody ? "" : responseBody });

  let isLoading = $state(false);

  // Function to make the API call
  async function makeApiCall() {
    response = null;
    responseBody = null;
    errorMessage = null;

    isLoading = true;

    try {
      let resp;
      if (methodInternal === "GET") {
        resp = await fetch(endpointInternal, {
          method: methodInternal,
          headers: headersJson.text ? JSON.parse(headersJson.text) : {},
        });
      } else {
        resp = await fetch(endpointInternal, {
          method: methodInternal,
          headers: headersJson.text ? JSON.parse(headersJson.text) : {},
          body: bodyJson.text,
        });
      }

      response = resp;

      let data = await response.text();
      responseBody = data;
    } catch (error) {
      errorMessage = error.message;
    }
    group = "response";
    isLoading = false;
  }

  $inspect(responseHeadersJson);
</script>

<div class="card preset-filled-surface-100-900 border-[1px] border-surface-200-800 p-4 text-left space-y-4">
  <div class="input-group divide-surface-200-800 grid-cols-[auto_1fr_auto] divide-x">
    <div class="input-group-cell bg-secondary-50 text-secondary-contrast-50 pr-0">
      <select bind:value={methodInternal} class="select">
        <option value="GET">GET</option>
        <option value="POST">POST</option>
        <option value="PUT">PUT</option>
        <option value="DELETE">DELETE</option>
      </select>
    </div>
    <input type="search" bind:value={endpointInternal} class="outline-none px-2" placeholder="Endpoint URL" />
    <div class="bg-secondary-500 text-secondary-contrast-500">
      <button class="btn preset-filled" onclick={makeApiCall}>
        <div class="relative">
          <!-- Text -->
          <span class={`transition-opacity duration-300 ${isLoading ? "opacity-0" : "opacity-100"}`}>
            {title}
          </span>

          <!-- Spinner -->
          <div class={`absolute inset-0 flex items-center justify-center transition-opacity duration-300 ${isLoading ? "opacity-100" : "opacity-0"}`}>
            <Icon icon="svg-spinners:180-ring" class="size-8" />
          </div>
        </div>
      </button>
    </div>
  </div>

  <div class="relative">
    <Tabs bind:value={group} composite>
      {#snippet list()}
        <Tabs.Control value="request">
          {#snippet lead()}
            <Icon icon="mdi:send" class="size-6 text-secondary-500" />
          {/snippet}
          Request
        </Tabs.Control>
        <Tabs.Control value="response">
          {#snippet lead()}
            <Icon icon="mdi:code-tags" class="size-6" />
          {/snippet}
          Response
        </Tabs.Control>
      {/snippet}
      {#snippet content()}
        <Tabs.Panel value="request">
          {#if methodInternal !== "GET"}
            <div class="grid grid-cols-2 gap-4">
              <div>
                <span class="badge preset-filled-secondary-500">Header</span>
                <div class="mt-2" style="height: 400px;">
                  <JSONEditor mode={Mode.text} mainMenuBar={false} statusBar={false} navigationBar={false} askToFormat={false} bind:content={headersJson} />
                </div>
              </div>

              <div>
                <span class="badge preset-filled-secondary-500">Body</span>
                <div class="mt-2" style="height: 400px;">
                  <JSONEditor mode={Mode.text} mainMenuBar={false} statusBar={false} navigationBar={false} askToFormat={false} bind:content={bodyJson} />
                </div>
              </div>
            </div>
          {:else}
            <div class="grid grid-cols-1 gap-4">
              <div>
                <span class="badge preset-filled-secondary-500">Header</span>
                <div class="mt-2" style="height: 400px;">
                  <JSONEditor mode={Mode.text} mainMenuBar={false} statusBar={false} navigationBar={false} askToFormat={false} bind:content={headersJson} />
                </div>
              </div>
            </div>
          {/if}
        </Tabs.Panel>
        <Tabs.Panel value="response">
          <div class="grid grid-cols-2 gap-4">
            <div>
              <span class="badge preset-filled-secondary-500">Header</span>
              <div class="mt-2" style="height: 400px;">
                <JSONEditor
                  mode={Mode.text}
                  mainMenuBar={false}
                  statusBar={false}
                  navigationBar={false}
                  askToFormat={false}
                  readOnly={true}
                  content={responseHeadersJson} />
              </div>
            </div>

            <div>
              <span class="badge preset-filled-secondary-500">Body</span>
              <div class="mt-2" style="height: 400px;">
                <JSONEditor
                  mode={Mode.text}
                  mainMenuBar={false}
                  statusBar={false}
                  navigationBar={false}
                  askToFormat={false}
                  readOnly={true}
                  content={responseBodyJson} />
              </div>
            </div>
          </div>
          <div>
            {#if errorMessage}
              <p class="text-error mt-2">Error: {errorMessage}</p>
            {/if}
          </div>
        </Tabs.Panel>
      {/snippet}
    </Tabs>
    <div class="absolute top-1 right-1">
      {#if response}
        <span class={`badge text-lg ${response.ok ? "preset-filled-success-500" : "preset-filled-error-700-300"}`}>
          {response.status} - {response.statusText}
        </span>
      {/if}
    </div>
  </div>
</div>

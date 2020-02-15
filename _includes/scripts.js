{% if site.footer_scripts %}
  {% for script in site.footer_scripts %}
    {% if script contains "://" %}
      {% capture script_path %}{{ script }}{% endcapture %}
    {% else %}
      {% capture script_path %}{{ script | relative_url }}{% endcapture %}
    {% endif %}
    <script src="{{ script_path }}"></script>
  {% endfor %}
{% else %}
  <script src="{{ '/assets/js/main.min.js' | relative_url }}"></script>
  <script src="https://kit.fontawesome.com/4eee35f757.js"></script>
{% endif %}

{% if site.search == true or page.layout == "search" %}
  {%- assign search_provider = site.search_provider | default: "lunr" -%}
  {%- case search_provider -%}
    {%- when "lunr" -%}
      {% include_cached search/lunr-search-scripts.html %}
    {%- when "google" -%}
      s
    {%- when "algolia" -%}
     a
  {%- endcase -%}
{% endif %}

{% if site.after_footer_scripts %}
  {% for script in site.after_footer_scripts %}
    {% if script contains "://" %}
      {% capture script_path %}{{ script }}{% endcapture %}
    {% else %}
      {% capture script_path %}{{ script | relative_url }}{% endcapture %}
    {% endif %}
    <script src="{{ script_path }}"></script>
  {% endfor %}
{% endif %}
